package com.samsung.dali.modelconverter.data.document;

import java.util.Map;
import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonProperty;

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

  public Primitive getPrimitive() {
    return sPrimitiveNames.get(mPrimitiveString);
  }

  public String getBinUri() {
    return mBinUri;
  }

  public int getAttributes() {
    return mAttributes;
  }

  public BufferRef getIndices() {
    return mIndices;
  }

  public BufferRef getPositions() {
    return mPositions;
  }

  public BufferRef getNormals() {
    return mNormals;
  }

  public BufferRef getUvs() {
    return mUvs;
  }

  public BufferRef getTangents() {
    return mTangents;
  }

  public BufferRef getBitangents() {
    return mBitangents;
  }

  @JsonProperty("uri")
  private String mBinUri;

  @JsonProperty("attributes")
  private int mAttributes = 0;

  @JsonProperty("primitive")
  private String mPrimitiveString;

  @JsonProperty("indices")
  private BufferRef mIndices;

  @JsonProperty("positions")
  private BufferRef mPositions;

  @JsonProperty("normals")
  private BufferRef mNormals;

  @JsonProperty("textures")
  private BufferRef mUvs;

  @JsonProperty("tangents")
  private BufferRef mTangents;

  @JsonProperty("bitangents")
  private BufferRef mBitangents;
}
