package com.samsung.dali.modelconverter.data.document;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

import java.util.ArrayList;
import java.util.Map;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Material {

  @Override
  public String toString() {
    String name = "";
    for (int i = 0; i < mTextures.length; ++i) {
      if (mTextures[i] != null) {
        if (!name.isEmpty()) {
          name += "^";
        }
        name += mTextures[i];
      }
    }
    return name;
  }

  public String getTexture(int id) {
    return mTextures[id];
  }

  public void setTextures(int id, String path) {
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
