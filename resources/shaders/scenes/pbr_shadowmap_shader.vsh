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

uniform highp mat4 uMvpMatrix;
uniform highp mat4 uViewMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uModelView;

uniform mat4 uLightSpaceTransform;
uniform vec3 uLightDir;

const float ShadowBias = .003;

void main()
{
  vec4 vPosition = vec4( aPosition, 1.0);
  gl_Position = uMvpMatrix * vPosition;
  vUV = aTexCoord;

  vNormal = normalize(uNormalMatrix * aNormal);

  vTangent = normalize(uNormalMatrix * aTangent);

  vBitangent = cross(vNormal, vTangent);

  vViewVec = (uModelView * vPosition).xyz;

  // Now cast a shadow.
  vec4 lightSpacePos = uLightSpaceTransform * uModelMatrix * vPosition;

  // Transform to NDC.
  vLightSpacePos.xy = 0.5 * (lightSpacePos.xy + lightSpacePos.ww);

  float shadowBias = dot(vNormal, uLightDir);
  shadowBias *= ShadowBias;
  vLightSpacePos.z = 0.5 * (lightSpacePos.z + lightSpacePos.w - shadowBias);
}
