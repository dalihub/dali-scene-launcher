package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Asset {

  public String getVersion() {
    return mVersion;
  }

  public void setVersion(String version) {
    mVersion = version;
  }

  private String mVersion;
}
