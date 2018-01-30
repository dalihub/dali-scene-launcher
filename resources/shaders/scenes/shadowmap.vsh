#version 300 es

precision mediump float;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform vec2 uNearFar;

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;
in vec3 aTangent;

out float vDepth;

void main()
{
	vec4 vPosition = uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	gl_Position = uProjection * vPosition;
//	vDepth = log2((vPosition.z - uNearFar.x) / (uNearFar.y - uNearFar.x)); 
	vDepth = (uNearFar.x - vPosition.z) / (uNearFar.x - uNearFar.y); 
}