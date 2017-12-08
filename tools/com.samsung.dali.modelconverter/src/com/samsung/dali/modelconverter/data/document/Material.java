package com.samsung.dali.modelconverter.data.document;

import java.util.Map;
import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Material {

  public String getTexture(int id) {
    return mTextures[id];
  }

  public void setTexture(int id, String path) {
    mTextures[id] = path;
  }

  @JsonIgnore
  public Integer getEnvironmentId() {
    return mEnvironmentId;
  }

  public void setEnvironmentId(Integer environmentId) {
    mEnvironmentId = environmentId;
  }

  public Environment getEnvironment(Document doc) {
    return getEnvironmentId() != null ? doc.getEnvironments().get(getEnvironmentId()) : null;
  }

  @JsonAnySetter
  public void setTexture(String key, String value) {
    if (key.startsWith("texture")) {
      int i = Integer.parseInt(key.substring("texture".length()));
      // NOTE: sneaky conversion from 1-based to 0-based.
      if (i > 0 && i <= mTextures.length) {
        mTextures[i - 1] = value;
      }
    }
  }

  @JsonAnyGetter
  public Map<String, String> getTextures() {
    Map<String, String> keyValues = new TreeMap<String, String>();
    for (int i = 0; i < mTextures.length; ++i) {
      if (mTextures[i] != null) {
        keyValues.put("texture" + (i + 1), mTextures[i]);
      }
    }
    return keyValues;
  }

  @JsonIgnore // custom JsonAnySetter / Getter.
  private String[] mTextures = new String[4];

  @JsonProperty("environment")
  private Integer mEnvironmentId;
}
