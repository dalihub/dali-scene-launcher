package com.samsung.dali.modelConverter.data.dlimodel;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Environment {

  @JsonProperty("cubeSpecular")
  public String specularPath;

  @JsonProperty("cubeDiffuse")
  public String diffusePath;


}
