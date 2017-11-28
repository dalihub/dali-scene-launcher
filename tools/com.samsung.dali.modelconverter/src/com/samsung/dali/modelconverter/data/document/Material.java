package com.samsung.dali.modelconverter.data.document;

import java.util.ArrayList;
import java.util.Map;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Material {

  public String getTexture(int id) {
    return mTextures[id];
  }

  public void setTextures(int id, String path) {
    mTextures[id] = path;
  }

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
      if (i >= 0 && i < mTextures.length) {
        mTextures[i] = value;
      }
    }
  }

  @JsonAnyGetter
  public Map<String, String> getTextures() {
    ArrayList<String[]> keyValues = new ArrayList<String[]>();
    for (int i = 0; i < mTextures.length; ++i) {
      if (mTextures[i] != null) {
        keyValues.add(new String[] { "texture" + (i + 1), mTextures[i] });
      }
    }
    return null;
  }

  @JsonIgnore // custom JsonAnySetter / Getter.
  private String[] mTextures = new String[4];

  @JsonProperty("environment")
  private Integer mEnvironmentId;
}
