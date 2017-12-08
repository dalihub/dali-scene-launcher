package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonSetter;

public class Environment {

  @JsonGetter("cubeSpecular")
  public String getSpecularPath() {
    return mSpecularPath;
  }

  @JsonSetter("cubeSpecular")
  public void setSpecularPath(String path) {
    mSpecularPath = path;
  }

  @JsonGetter("cubeDiffuse")
  public String getDiffusePath() {
    return mDiffusePath;
  }

  @JsonSetter("cubeDiffuse")
  public void setDiffusePath(String path) {
    mDiffusePath = path;
  }

  private String mSpecularPath;
  private String mDiffusePath;
}
