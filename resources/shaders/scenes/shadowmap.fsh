#version 300 es

precision mediump float;

in float vDepth;
in float vDepthSqr;

// NOTE: only writing 4 channels to keep the debugging of shadowmaps (SHADOWMAPS_CAST only) working; otherwise writing RGB only would suffice.
out vec4 FragColor;

void main()
{
  // Variance Shadow Maps - prefilter depth [squared] with values from adjacent.
  // More info: http://www.fabiensanglard.net/shadowmappingVSM/
  float dx = dFdx(vDepth);
  float dy = dFdy(vDepth);
  float moment2 = vDepthSqr + 0.25 * (dx * dx + dy * dy);

  FragColor = vec4(vDepth, moment2, .0, 1.0);
}
