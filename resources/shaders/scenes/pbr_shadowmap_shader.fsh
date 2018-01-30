#version 300 es

precision mediump float;

uniform sampler2D sAlbedoMetal;
uniform sampler2D sNormalRoughness;
uniform samplerCube sDiffuse;
uniform samplerCube sSpecular;
uniform sampler2D sShadow;

// Transformation matrix of the cubemap texture

// Number of mip map levels in the texture
uniform float uMaxLOD;

uniform mat4 uCubeMatrix;

uniform vec2 uInverseShadowmapSize;

in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vViewVec;
in vec3 vLightSpacePos;

out vec4 FragColor;

// Functions for BRDF calculation come from
// https://www.unrealengine.com/blog/physically-based-shading-on-mobile
// Based on the paper by Dimitar Lazarov
// http://blog.selfshadow.com/publications/s2013-shading-course/lazarov/s2013_pbs_black_ops_2_notes.pdf
vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
  const vec4 c0 = vec4( -1.0, -0.0275, -0.572, 0.022 );
  const vec4 c1 = vec4( 1.0, 0.0425, 1.04, -0.04 );
  vec4 r = Roughness * c0 + c1;
  float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
  vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;

  return SpecularColor * AB.x + AB.y;
}

const float Gamma = 2.2;

// Determine whether we're shadowed or not; use as a modifier to the color, assuming
// a shadow colour of black.
const float MinVariance = pow(2.0, -9.0);
const vec2 PoissonDisc[] = vec2[5](  // x, y - offset in texels
  vec2(.0, .0),
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, 1.0)
);

float CalculateShadow(vec3 lightSpaceProjectedPos)
{
  float shadowAccum = .0;
  float totalWeight = .0;

  // Use Percentage-Closer Filtering for additional smoothing of shadows.
  // Normally prefiltering (VSM) would be sufficient, however this was found
  // to improve the quality of shadows.
  for(int i = 0; i < 5; ++i)
  {
    vec2 moments = texture(sShadow, lightSpaceProjectedPos.xy + PoissonDisc[i].xy * uInverseShadowmapSize).rg;
    float distance = lightSpaceProjectedPos.z;
    if (distance <= moments.x)  // Fragment is in front of any occluders
    {
      shadowAccum += 1.0;
    }
    else // Variance Shadow Maps -- calculate probability of occlusion using Chebyshev's formula.
         // More info: http://www.fabiensanglard.net/shadowmappingVSM/
    {
      float variance = moments.y - (moments.x * moments.x);
      variance = max(variance, MinVariance);

      distance -= moments.x;
      float probability = variance / (variance + distance * distance);

      shadowAccum += probability;
    }
    totalWeight += 1.0;
  }

  if(totalWeight > .0)
  {
    shadowAccum /= totalWeight; // NOTE: once we've settled on a sample size, just make sure the weights add up to 1 and save the division.

    shadowAccum *= shadowAccum; // Darker shadows / penumbra
    shadowAccum *= shadowAccum; // pow(shadowAccum, 4.0)
  }

  return shadowAccum;  
}

void main()
{
  // We get information from the maps (albedo, normal map, roughness, metalness
  // I access the maps in the order they will be used
  vec4 normalRoughness = texture(sNormalRoughness, vUV.st);
  vec3 normalMap = normalRoughness.rgb;
  float roughness = normalRoughness.a;

  vec4 albedoMetal = texture(sAlbedoMetal, vUV.st);
  vec3 albedoColor = albedoMetal.rgb;
  float metallic = albedoMetal.a;
  
  //Normalize vectors
  vec3 normal = normalize(vNormal);
  vec3 tangent = normalize(vTangent);
  vec3 bitangent = normalize(vBitangent);

  vec3 viewVec = normalize(vViewVec);

  // Create Inverse Local to world matrix
  mat3 vInvTBN = mat3(tangent, bitangent, normal);

  // Get normal map info in world space
  normalMap = normalize((normalMap - 0.5) * 2.0);
  vec3 newNormal = vInvTBN * normalMap.rgb;

  // Calculate normal dot view vector
  float NoV = clamp(dot(newNormal, -viewVec), 0.0, 1.0);

  // Reflect vector
  vec3 reflectionVec = reflect(viewVec, newNormal);

  //transform it now to environment coordinates (used when the environment rotates)
  vec3 reflecCube = (uCubeMatrix * vec4( reflectionVec, 0 ) ).xyz;
  reflecCube = normalize( reflecCube );

  //transform it now to environment coordinates
  vec3 normalCube = ( uCubeMatrix * vec4( newNormal, 0 ) ).xyz;
  normalCube = normalize( normalCube );

  // Get irradiance from diffuse cubemap
  vec3 irradiance = texture( sDiffuse, normalCube ).rgb;

  // Way to access the mip map level - copied from UE
  const float REFLECTION_CAPTURE_ROUGHEST_MIP = 1.;
  const float REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE = 1.2;
  float LevelFrom1x1 = REFLECTION_CAPTURE_ROUGHEST_MIP - REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE * log2(roughness);
  // Note: must match GReflectionCaptureSize
  // We need to subtract 2 levels from the uMaxLOD because we don't have the smallest mipmaps
  // (and because these levels are very small 1x1, 2x2, etc.)
  float finalLod = (uMaxLOD - 2.0) - 1.0 - LevelFrom1x1;

  // calculate the convexity and increase the reflectance lobe
  float convexity = length(fwidth(newNormal));
  finalLod += convexity * (uMaxLOD - 2.0);

  finalLod = mix( 0.0, uMaxLOD, roughness);
  // Access reflection color using roughness value
  vec3 reflectionColor = textureLod(sSpecular, reflecCube, finalLod).rgb;

  float shadowComponent = CalculateShadow(vLightSpacePos);
  
  // For increasing contrast a little bit - It should be Gamma correction (2.2) but it gets to dark
  albedoColor *= shadowComponent;

  // We use DielectricColor (0.04) of a plastic (almost everything)
  // http://blog.selfshadow.com/publications/s2014-shading-course/hoffman/s2014_pbs_physics_math_slides.pdf

  vec3 DiffuseColor = albedoColor - albedoColor * metallic;  // 1 mad
  vec3 SpecularColor = mix( vec3(0.04), albedoColor , metallic); // 2 mad

  // Calculate specular color using Magic Function (takes original rougness and normal dot view
  vec3 specColor =  reflectionColor.rgb * EnvBRDFApprox(SpecularColor, roughness, NoV );// * irradiance;

  // Multiply the result by albedo texture and do energy conservation
  vec3 diffuseColor = (irradiance) * DiffuseColor;
  // Final color is the sum of the diffuse and specular term
  vec3 finalColor = diffuseColor + specColor;

  //finalColor = finalColor / (finalColor + vec3(1.0) );

  finalColor = pow( finalColor, vec3( 1.0 / 2.2 ) );
  FragColor = vec4( finalColor, 1.0 );
}
