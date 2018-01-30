#version 300 es

precision mediump float;

in float vDepth;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vec3(vDepth), 1.0);
}