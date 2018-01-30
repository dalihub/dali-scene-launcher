#version 300 es

precision mediump float;

uniform sampler2D sAlbedoAlpha;
uniform sampler2D sMetalRoughness;
uniform sampler2D sNormal;
uniform samplerCube sSpecular;

// Transformation matrix of the cubemap texture

// Number of mip map levels in the texture
uniform float uMaxLOD;
uniform vec3 uDiffuse;    // The irradiance
uniform mat4 uCubeMatrix;
uniform vec4 uColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vViewVec;

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

void main()
{
  // We get information from the maps (albedo, normal map, roughness, metalness
  // I access the maps in the order they will be used
  vec4 metalRoughness = texture(sMetalRoughness, vUV.st);
  vec3 normalMap = texture(sNormal, vUV.st).rgb;
  float roughness = metalRoughness.a;

  vec4 albedoAlpha = texture(sAlbedoAlpha, vUV.st);
  vec3 albedoColor = albedoAlpha.rgb;
  float metallic = metalRoughness.r;
  float alpha = albedoAlpha.a;

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

  float finalLod = mix( 0.0, uMaxLOD, roughness);
  // Access reflection color using roughness value
  vec3 reflectionColor = textureLod(sSpecular, reflecCube, finalLod).rgb;

  // We use DielectricColor (0.04) of a plastic (almost everything)
  // http://blog.selfshadow.com/publications/s2014-shading-course/hoffman/s2014_pbs_physics_math_slides.pdf

  vec3 DiffuseColor = albedoColor - albedoColor * metallic;  // 1 mad
  vec3 SpecularColor = mix( vec3(0.04), albedoColor , metallic); //2 mad

  // Calculate specular color using Magic Function (takes original rougness and normal dot view
  vec3 specColor =  reflectionColor.rgb * EnvBRDFApprox(SpecularColor, roughness, NoV );

  // Multiply the result by albedo texture and do energy conservation
  vec3 diffuseColor = uDiffuse * DiffuseColor;
  // Final color is the sum of the diffuse and specular term
  vec3 finalColor = diffuseColor + specColor;

  finalColor = sqrt( 3.0 * finalColor ); // Approximation of the gamma correction pow( 3.0 * finalColor, 1/2.2 )
  FragColor = vec4( finalColor, alpha * uColor.a );
}
