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
out vec4 vLightSpacePos;

//uniform vec2 uDirection;
//uniform vec3 uSize;

uniform mat3 uNormalMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uModelView;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;

uniform mat4 uLightSpaceTransform;

void main()
{
  vec4 vPosition = uModelMatrix * vec4( aPosition, 1.0);
  gl_Position = uProjection * uViewMatrix * vPosition;
  vUV = aTexCoord;

  vNormal = normalize(uNormalMatrix * aNormal);

  vTangent = normalize(uNormalMatrix * aTangent);

  vBitangent = cross(vNormal, vTangent);

  vViewVec = (uViewMatrix * vPosition).xyz;

  vLightSpacePos = uLightSpaceTransform * vPosition;
}
