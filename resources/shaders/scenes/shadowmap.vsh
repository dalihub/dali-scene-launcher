#version 300 es

precision mediump float;

uniform mat4 uMvpMatrix;
uniform vec2 uNearFar;

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;
in vec3 aTangent;

out float vDepth;

void main()
{
	vec4 vPosition = uMvpMatrix * vec4(aPosition, 1.0);
	gl_Position = vPosition;
//	vDepth = log2((vPosition.z - uNearFar.x) / (uNearFar.y - uNearFar.x)); 
	vDepth = (uNearFar.x - vPosition.z) / (uNearFar.x - uNearFar.y); 
}