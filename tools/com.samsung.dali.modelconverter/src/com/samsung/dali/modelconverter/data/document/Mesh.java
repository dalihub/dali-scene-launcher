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

import java.util.Map;
import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.fasterxml.jackson.annotation.JsonSetter;

@JsonPropertyOrder({ "uri", "attributes", "primitive", "positions", "normals", "textures", "tangents", "bitangents" })
public class Mesh {

  public enum Primitive {
    TRIANGLES, LINES, POINTS
  }

  static final int idMask(int bit) {
    return 1 << bit;
  }

  public static final int INDICES_BIT = idMask(0);
  public static final int POSITIONS_BIT = idMask(1);
  public static final int NORMALS_BIT = idMask(2);
  public static final int UV_BIT = idMask(3);
  public static final int TANGENTS_BIT = idMask(4);
  public static final int BITANGENTS_BIT = idMask(5);

  public static Map<String, Primitive> sPrimitiveNames;
  static {
    sPrimitiveNames = new TreeMap<String, Primitive>();
    sPrimitiveNames.put("TRIANGLES", Primitive.TRIANGLES);
    sPrimitiveNames.put("LINES", Primitive.LINES);
    sPrimitiveNames.put("POINTS", Primitive.POINTS);
  }

  @Override
  public String toString() {
    String name = mBinUri;
    if (mPositions != null) {
      name += " @" + mPositions.mByteOffset + "[" + mPositions.mByteLength + "]";
    }

    String flags = "";
    if (mIndices != null) {
      flags += "I";
    }
    if (mUvs != null) {
      flags += "U";
    }
    if (mNormals != null) {
      flags += "N";
    }
    if (mTangents != null) {
      flags += "T";
    }
    if (mBitangents != null) {
      flags += "B";
    }
    if (flags.length() > 0) {
      name += " " + flags;
    }

    return name;
  }

  public Primitive getPrimitive() {
    return mPrimitive;
  }

  public void setPrimitive(Primitive prim) {
    mPrimitive = prim;
  }

  @JsonIgnore
  public String getBinUri() {
    return mBinUri;
  }

  public int getAttributes() {
    return mAttributes;
  }

  public void setAttributes(int a) {
    mAttributes = a;
  }

  public BufferRef getIndices() {
    return mIndices;
  }

  public void setIndices(BufferRef br) {
    mIndices = br;
  }

  public BufferRef getPositions() {
    return mPositions;
  }

  public void setPositions(BufferRef br) {
    mIndices = br;
  }

  public BufferRef getNormals() {
    return mNormals;
  }

  public void setNormals(BufferRef br) {
    mIndices = br;
  }

  @JsonGetter("textures")
  public BufferRef getUvs() {
    return mUvs;
  }

  @JsonSetter("textures")
  public void setUvs(BufferRef br) {
    mUvs = br;
  }

  public BufferRef getTangents() {
    return mTangents;
  }

  public void setTangents(BufferRef br) {
    mTangents = br;
  }

  public BufferRef getBitangents() {
    return mBitangents;
  }

  public void setBitangents(BufferRef br) {
    mBitangents = br;
  }

  @JsonProperty("uri")
  private String mBinUri;

  private int mAttributes = 0;

  private Primitive mPrimitive = Primitive.TRIANGLES;

  private BufferRef mIndices;
  private BufferRef mPositions;
  private BufferRef mNormals;
  private BufferRef mUvs;
  private BufferRef mTangents;
  private BufferRef mBitangents;
}