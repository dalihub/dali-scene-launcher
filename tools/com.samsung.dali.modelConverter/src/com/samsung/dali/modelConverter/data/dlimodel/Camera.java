package com.samsung.dali.modelConverter.data.dlimodel;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Camera {

  @JsonProperty("fov")
  public float flov;
  
  @JsonProperty("near")
  public float near;
  
  @JsonProperty("far")
  public float far;
  
  @JsonProperty("matrix")
  public float[] matrix = new float [16];

}
