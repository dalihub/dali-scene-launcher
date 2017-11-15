package com.samsung.dali.modelConverter.data.dlimodel;

import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Mesh implements DliModel.IdResolver {

  public enum Primitive
  {
    TRIANGLES,
    LINES,
    POINTS
  }
  
  static TreeMap<String, Primitive> primitiveNames;
  static {
    primitiveNames = new TreeMap<String, Primitive>();
    primitiveNames.put("TRIANGLES", Primitive.TRIANGLES);
    primitiveNames.put("LINES", Primitive.LINES);
    primitiveNames.put("POINTS", Primitive.POINTS);
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
  
  @JsonProperty("uri")
  public String binUri;
  
  @JsonProperty("attributes")
  public int attributes = 0;

  @JsonProperty("primitive")
  public String primitiveString;
  
  @JsonIgnore
  public Primitive primitive;

  @JsonProperty("indices")
  public BufferRef indices;

  @JsonProperty("positions")
  public BufferRef positions;

  @JsonProperty("normals")
  public BufferRef normals;

  @JsonProperty("textures")
  public BufferRef uvs;

  @JsonProperty("tangents")
  public BufferRef tangents;

  @JsonProperty("bitangents")
  public BufferRef bitangents;

  @Override
  public void resolveIds(DliModel doc) {
    
    // NOTE / TODO: bit of a subversion, this one. Jackson might be able to parse an enum; investigate later.
    primitive = primitiveNames.get(primitiveString);
  }

  @Override
  public void updateIds(DliModel doc) {
    
    // NOTE / TODO: bit of a subversion, this one. Jackson might be able to parse an enum; investigate later.
    primitiveString = primitive.name();
  }
  
}
