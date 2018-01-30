#version 300 es

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;
in vec3 aTangent;

out vec2 vUV;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vViewVec;
out vec3 vLightSpacePos;

//uniform vec2 uDirection;
//uniform vec3 uSize;

uniform mat4 uMvpMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uModelView;
uniform mat4 uViewMatrix;

uniform mat4 uLightSpaceTransform;

void main()
{
  vec4 vPosition = vec4( aPosition, 1.0);
  gl_Position = uMvpMatrix * vPosition;
  vUV = aTexCoord;

  vNormal = normalize(uNormalMatrix * aNormal);

  vTangent = normalize(uNormalMatrix * aTangent);

  vBitangent = cross(vNormal, vTangent);

  vViewVec = (uModelView * vPosition).xyz;

  vec4 lightSpacePos = uLightSpaceTransform * uModelMatrix * vPosition;

  // Perform manual perspective divide
  lightSpacePos.xyz /= lightSpacePos.w;

  // Transform to NDC.
  vLightSpacePos = lightSpacePos.xyz * .5 + .5;

}
