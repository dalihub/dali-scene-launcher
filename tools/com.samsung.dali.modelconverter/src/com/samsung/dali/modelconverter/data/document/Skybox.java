package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Skybox {

  public String getTexture() {
    return mTexture;
  }

  public void setTexture(String mTexture) {
    this.mTexture = mTexture;
  }

  @JsonProperty("texture")
  private String mTexture;
}
