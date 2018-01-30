#version 300 es

precision mediump float;

in float vDepth;
in float vDepthSqr;

out vec3 FragColor;

void main()
{
  float dx = dFdx(vDepth);
  float dy = dFdy(vDepth);
  float moment2 = vDepthSqr + 0.25 * (dx * dx + dy * dy);
  FragColor = vec3(vDepth, moment2, .0);
}