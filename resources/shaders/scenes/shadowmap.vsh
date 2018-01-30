#version 300 es

precision mediump float;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform vec2 uNearFar;  // x is distance of near clipping plane; y is 1.0 / (zFar - zNear) 

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;
in vec3 aTangent;

out highp float vDepth;
out highp float vDepthSqr;

void main()
{
	vec4 vPosition = uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	gl_Position = uProjection * vPosition;
	vDepth = (vPosition.z - uNearFar.x) * uNearFar.y;
	vDepthSqr = vDepth * vDepth;
}