package com.samsung.dali.modelconverter.data.document;

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
