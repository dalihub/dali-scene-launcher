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
import java.util.Map.Entry;
import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonSetter;
import com.samsung.dali.modelconverter.data.document.valueholders.FloatHolder;
import com.samsung.dali.modelconverter.data.document.valueholders.IntHolder;
import com.samsung.dali.modelconverter.data.document.valueholders.MatrixHolder;
import com.samsung.dali.modelconverter.data.document.valueholders.VectorHolder;

public class Shader {

  @JsonCreator
  public Shader(@JsonProperty("vertex") String vertexPath, @JsonProperty("fragment") String fragmentPath,
      @JsonProperty("renderMode") int renderMode) {
    mVertexPath = vertexPath;
    mFragmentPath = fragmentPath;
    mRenderMode = renderMode;
  }

  public void setId( int id )
  {
    mId = id;
  }

  public int getId()
  {
    return mId;
  }

  @Override
  public String toString() {
    return mId + " " + mVertexPath + "^" + mFragmentPath + "^" + mRenderMode;
  }

  @JsonGetter("vertex")
  public String getVertexPath() {
    return mVertexPath;
  }

  @JsonSetter("vertex")
  public void setVertexPath(String mVertexPath) {
    this.mVertexPath = mVertexPath;
  }

  @JsonGetter("fragment")
  public String getFragmentPath() {
    return mFragmentPath;
  }

  @JsonSetter("fragment")
  public void setFragmentPath(String mFragmentPath) {
    this.mFragmentPath = mFragmentPath;
  }

  public int getRenderMode() {
    return mRenderMode;
  }

  public void setRenderMode(int mRenderMode) {
    this.mRenderMode = mRenderMode;
  }

  public ValueHolder getUniform(String name) {
    return (ValueHolder) mUniforms.get(name);
  }

  public void setUniform(String name, ValueHolder u) {
    mUniforms.put(name, u);
  }

  @JsonAnySetter
  public void add(String key, Object value) {
    // Attempt to add the right type of uniform.
    if (value instanceof Integer) {
      mUniforms.put(key, new IntHolder((Integer) value));
    }
    else if (value instanceof Number) {
      mUniforms.put(key, new FloatHolder((Number) value));
    }
    else if (value instanceof ArrayList<?>) {
      ArrayList<?> arrayList = (ArrayList<?>) value;

      if (arrayList.isEmpty()) {
        throw new IllegalArgumentException("Failed to create uniform from empty array. What type?");
      }

      for (Object o : arrayList) {
        if (!(o instanceof Number)) {
          throw new IllegalArgumentException("Failed to create uniform from non-numeric type " + o.getClass().getName()
              + " @ index " + arrayList.indexOf(o));
        }
      }

      switch (arrayList.size()) {
      case 1:
      case 2:
      case 3:
      case 4:
        mUniforms.put(key, new VectorHolder(arrayList.toArray(new Number[0])));
        break;

      case 9:
      case 16:
        mUniforms.put(key, new MatrixHolder(arrayList.toArray(new Number[0])));
        break;

      default:
        throw new IllegalArgumentException("Unsupported number of elements in uniform: " + arrayList.size());
      }
    }
    else {
      throw new IllegalArgumentException("Unknown type: " + value.getClass().getName());
    }
  }

  private int mId = 0;

  @JsonAnyGetter
  public Map<String, Object> get() {
    Map<String, Object> map = new TreeMap<String, Object>();
    for (Entry<String, ValueHolder> e : mUniforms.entrySet()) {
      map.put(e.getKey(), e.getValue().get());
    }
    return map;
  }

  private String mVertexPath;
  private String mFragmentPath;
  private int mRenderMode;
  private Map<String, ValueHolder> mUniforms = new TreeMap<String, ValueHolder>();
}
