package com.samsung.dali.modelconverter.data.document;

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
import com.samsung.dali.modelconverter.data.document.uniforms.FloatUniform;
import com.samsung.dali.modelconverter.data.document.uniforms.IntUniform;
import com.samsung.dali.modelconverter.data.document.uniforms.MatrixUniform;
import com.samsung.dali.modelconverter.data.document.uniforms.VectorUniform;

public class Shader {

  @JsonCreator
  public Shader(@JsonProperty("vertex") String vertexPath, @JsonProperty("fragment") String fragmentPath,
      @JsonProperty("renderMode") int renderMode) {
    mVertexPath = vertexPath;
    mFragmentPath = fragmentPath;
    mRenderMode = renderMode;
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

  public Uniform getUniform(String name) {
    return (Uniform) mUniforms.get(name);
  }

  public void setUniform(String name, Uniform u) {
    mUniforms.put(name, u);
  }

  @JsonAnySetter
  public void add(String key, Object value) {
    // Attempt to add the right type of uniform.
    if (value instanceof Integer) {
      mUniforms.put(key, new IntUniform((Integer) value));
    }
    else if (value instanceof Number) {
      mUniforms.put(key, new FloatUniform((Number) value));
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
        mUniforms.put(key, new VectorUniform(arrayList.toArray(new Number[0])));
        break;

      case 9:
      case 16:
        mUniforms.put(key, new MatrixUniform(arrayList.toArray(new Number[0])));
        break;

      default:
        throw new IllegalArgumentException("Unsupported number of elements in uniform: " + arrayList.size());
      }
    }
    else {
      throw new IllegalArgumentException("Unknown type: " + value.getClass().getName());
    }
  }

  @JsonAnyGetter
  public Map<String, Object> get() {
    Map<String, Object> map = new TreeMap<String, Object>();
    for (Entry<String, Uniform> e : mUniforms.entrySet()) {
      map.put(e.getKey(), e.getValue().getValue());
    }
    return map;
  }

  private String mVertexPath;
  private String mFragmentPath;
  private int mRenderMode;
  private Map<String, Uniform> mUniforms = new TreeMap<String, Uniform>();
}
