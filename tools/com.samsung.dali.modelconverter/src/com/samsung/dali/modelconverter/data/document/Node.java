package com.samsung.dali.modelconverter.data.document;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.fasterxml.jackson.annotation.JsonSetter;

@JsonPropertyOrder({ "name", "matrix", "mesh", "material", "shader", "children" })
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

  @JsonIgnore
  public Integer getMaterialId() {
    return mMaterialId;
  }

  public void setMaterialId(int id) {
    mMaterialId = id;
  }

  public Material getMaterial(Document doc) {
    return mMaterialId != null ? doc.getMaterials().get(mMaterialId) : null;
  }

  @JsonIgnore
  public Integer getShaderId() {
    return mShaderId;
  }

  public void setShaderId(int id) {
    mShaderId = id;
  }

  public Shader getShader(Document doc) {
    return mShaderId != null ? doc.getShaders().get(mShaderId) : null;
  }

  @JsonIgnore
  public Integer getMeshId() {
    return mMeshId;
  }

  public void setMeshId(int id) {
    mMeshId = id;
  }

  public Mesh getMesh(Document doc) {
    return mMeshId != null ? doc.getMeshes().get(mMeshId) : null;
  }

  @JsonIgnore
  public Node getParent() {
    return (mParent == null) ? null : mParent.get();
  }

  public void setParent(Node n) {
    mParent = ((n == null) ? null : new WeakReference<Node>(n));
  }

  @JsonIgnore
  public ArrayList<Integer> getChildIds() {
    return mChildIds;
  }

  public void setChildIds(ArrayList<Integer> mChildIds) {
    this.mChildIds = mChildIds;
  }

  @JsonSetter("matrix")
  public void setMatrix(double[] data) {
    if (data != null) {
      mMatrix = MatrixHelper.createMatrix(data);
    }
    else {
      // Null matrix means identity.
      mMatrix = MatrixHelper.createMatrix();
    }
  }

  @JsonGetter("matrix")
  public double[] getMatrix() {
    return mMatrix;
  }

  private String mName = "NAME_MISSING";

  private double[] mMatrix = MatrixHelper.createMatrix();

  @JsonProperty("mesh")
  private Integer mMeshId;

  @JsonProperty("material")
  private Integer mMaterialId;

  @JsonProperty("shader")
  private Integer mShaderId;

  @JsonProperty("children")
  private ArrayList<Integer> mChildIds = new ArrayList<Integer>();

  private WeakReference<Node> mParent;
}
