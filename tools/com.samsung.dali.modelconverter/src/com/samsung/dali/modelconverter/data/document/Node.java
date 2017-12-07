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

import java.lang.ref.WeakReference;
import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonSetter;
import com.samsung.dali.modelconverter.data.document.property.IdList;
import com.samsung.dali.modelconverter.data.document.property.NullOffsetter;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class Node implements Property.Provider {

  @Override
  public String toString() {
    return mName;
  }

  @JsonIgnore
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

  public void setMaterialId(Integer id) {
    mMaterialId = id;
  }

  public Material getMaterial(Document doc) {
    return mMaterialId != null ? doc.getMaterials().get(mMaterialId) : null;
  }

  @JsonIgnore
  public Integer getShaderId() {
    return mShaderId;
  }

  public void setShaderId(Integer id) {
    mShaderId = id;
  }

  public Shader getShader(Document doc) {
    return mShaderId != null ? doc.getShaders().get(mShaderId) : null;
  }

  @JsonIgnore
  public Integer getMeshId() {
    return mMeshId;
  }

  public void setMeshId(Integer id) {
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

  @Override
  public void provideProperties(Document context, Property.Receiver receiver) {
    try {
      receiver.register("Name", new Property(this, "Name", Property.Type.String, true));
      receiver.register("Transform", new Property(this, "Matrix", Property.Type.Transform, false));
      receiver.register("Mesh", new Property(this, "MeshId", Property.Type.Id, true,
          new IdList<Mesh>(context.getMeshes(), true), new NullOffsetter()));
      receiver.register("Material", new Property(this, "MaterialId", Property.Type.Id, true,
          new IdList<Material>(context.getMaterials(), true), new NullOffsetter()));
      receiver.register("Shader", new Property(this, "ShaderId", Property.Type.Id, true,
          new IdList<Shader>(context.getShaders(), true), new NullOffsetter()));
    }
    catch (NoSuchFieldException | NoSuchMethodException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  @JsonProperty("name")
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
