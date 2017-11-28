package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Environment {

  @JsonProperty("cubeSpecular")
  public String getSpecularPath() {
    return mSpecularPath;
  }

  public void setSpecularPath(String path) {
    mSpecularPath = path;
  }

  @JsonProperty("cubeDiffuse")
  public String getDiffusePath() {
    return mDiffusePath;
  }

  public void setDiffusePath(String path) {
    mDiffusePath = path;
  }

  private String mSpecularPath;
  private String mDiffusePath;
}
