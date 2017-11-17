package com.samsung.dali.modelConverter.model.document;

import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonSetter;

public class Node implements Document.IdResolver {

  @JsonProperty("name")
  public String name;
  
  @JsonIgnore
  Matrix matrix = new Matrix();
  
  @JsonSetter("matrix")
  void setMatrix(double[] data)
  {
    matrix = new Matrix();
    if(data.length != matrix.data.length)
    {
      throw new IllegalArgumentException("Matrix requires an array of 16 floating point values.");
    }

    for(int i = 0; i < matrix.data.length; ++i)
    {
      matrix.data[i] = data[i];
    }
  }
  
  @JsonGetter("matrix")
  double[] getMatrix()
  {
    return matrix == null ? null : matrix.data;
  }
  
  @JsonProperty("mesh")
  public int meshId = -1;
  
  @JsonIgnore
  public Mesh mesh;
  
  @JsonProperty("material")
  public int materialId = -1;
  
  @JsonIgnore
  public Material material;
  
  @JsonProperty("shader")
  public int shaderId = -1;
  
  @JsonIgnore
  public Shader shader;

  @JsonProperty("children")
  public ArrayList<Integer> childIds = new ArrayList<Integer>();
  
  @JsonIgnore
  public ArrayList<Node> children;

  @Override
  public String toString()
  {
    return name;
  }
  
  @Override
  public void resolveIds(Document doc) {
    if(meshId != -1)
    {
      mesh = doc.meshes.get(meshId);
    }
    
    if(materialId != -1)
    {
      material = doc.materials.get(materialId);
    }
    
    if(shaderId != -1)
    {
      shader = doc.shaders.get(shaderId);
    }
    
    children = new ArrayList<Node>();
    for (Integer i: childIds)
    {
      Node n = doc.nodes.get(i.intValue());
      if(n != this)
      {
        children.add(n);
      }
    }
  }

  @Override
  public void updateIds(Document doc) {
    if(mesh != null)
    {
      meshId = doc.meshes.indexOf(mesh);
    }
    else
    {
      meshId = -1;
    }

    if(material != null)
    {
      materialId = doc.materials.indexOf(material);
    }
    else
    {
      materialId = -1;
    }

    if(shader != null)
    {
      shaderId = doc.shaders.indexOf(shader);
    }
    else
    {
      shaderId = -1;
    }

    childIds = new ArrayList<Integer>();
    for (Node n: children)
    {
      Integer i = new Integer(doc.nodes.indexOf(n));
      childIds.add(i);
    }
  }

  
}
