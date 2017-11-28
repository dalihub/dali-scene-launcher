package com.samsung.dali.modelconverter.data.document;

import java.util.ArrayList;
import java.util.Map;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonSetter;

public class Node {

  @Override
  public String toString() {
    return mName;
  }

  public String getName() {
  	return mName;
  }

  public void setName(String name) {
  	mName = name;
  }

  public Integer getMaterialId() {
    return mMaterialId;
  }

  public void setMaterialId(int id) {
    mMaterialId = id;
  }

  public Material getMaterial(Document doc) {
    return mMaterialId != null ? doc.getMaterials().get(mMaterialId) : null;
  }

  public Integer getShaderId() {
    return mShaderId;
  }

  public void setShaderId(int id) {
    mShaderId = id;
  }

  public Shader getShader(Document doc) {
    return mShaderId != null ? doc.getShaders().get(mShaderId) : null;
  }

  public Integer getMeshId() {
    return mMeshId;
  }

  public void setMeshId(int id) {
    mMeshId = id;
  }

  public Mesh getMesh(Document doc) {
    return mMeshId != null ? doc.getMeshes().get(mMeshId) : null;
  }

  public Node getParent() {
  	return mParent;
  }

  public void setParent(Node n) {
  	mParent = n;
  }

  public void collect(Document doc, String path, Map<String, Node> map)
  {
    assert doc != null;
    assert path != null;

    // Register current node
    path += mName;
    map.put(path, this);

    // Register children
    path += ":";
    for(Integer i: getChildIds())
    {
      if(i != null)
      {
        Node n = doc.getNode(i.intValue());
        n.collect(doc, path, map);
      }
    }
  }

	public ArrayList<Integer> getChildIds() {
		return mChildIds;
	}

	public void setChildIds(ArrayList<Integer> mChildIds) {
		this.mChildIds = mChildIds;
	}

  @JsonSetter("matrix")
  public void setMatrix(double[] data) {
  	mMatrix = MatrixHelper.createMatrix(data);
  }

  @JsonGetter("matrix")
  public double[] getMatrix() {
    return mMatrix;
  }

	@JsonProperty("name")
  private String mName;

  @JsonIgnore	// custom setter / getter provided
  private double[] mMatrix;

  @JsonProperty("mesh")
  private Integer mMeshId;

  @JsonProperty("material")
  private Integer mMaterialId;

  @JsonProperty("shader")
  private Integer mShaderId;

  @JsonProperty("children")
  private ArrayList<Integer> mChildIds = new ArrayList<Integer>();

  @JsonIgnore
	private Node mParent;

}
