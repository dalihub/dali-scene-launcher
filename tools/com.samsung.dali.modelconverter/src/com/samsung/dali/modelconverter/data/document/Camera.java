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

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class Camera implements Property.IProvider {

  @JsonIgnore
  public int getId() {
    return mId;
  }

  public void setId(int id) {
    mId = id;
  }

  @Override
  public String toString() {
    return "Camera " + mId;
  }

  public double getFov() {
    return mFov;
  }

  public void setFov(Number fov) {
    mFov = fov.doubleValue();
  }

  public double getNear() {
    return mNear;
  }

  public void setNear(Number near) {
    mNear = near.doubleValue();
  }

  public double getFar() {
    return mFar;
  }

  public void setFar(Number far) {
    mFar = far.doubleValue();
  }

  public double[] getMatrix() {
    return mMatrix;
  }

  public void setMatrix(double[] mtx) {
    assert mtx == null || mtx.length == 16;
    mMatrix = mtx;
  }

  @Override
  public void provideProperties(Document context, Property.IReceiver receiver) {
    try {
      receiver.register("Transform", new Property(this, "Matrix", Property.Type.Transform, true));
      receiver.register("Field of View", new Property(this, "Fov", Property.Type.Number, true));
      receiver.register("Near plane", new Property(this, "Near", Property.Type.Number, true));
      receiver.register("Far plane", new Property(this, "Far", Property.Type.Number, true));
    }
    catch (NoSuchFieldException | NoSuchMethodException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  private double mFov;
  private double mNear;
  private double mFar;
  private double[] mMatrix = MatrixHelper.createMatrix();

  @JsonIgnore
  private int mId;
}
