package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonIgnore;

public class Camera {

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

  public void setFov(double fov) {
    mFov = fov;
  }

  public double getNear() {
    return mNear;
  }

  public void setNear(double near) {
    mNear = near;
  }

  public double getFar() {
    return mFar;
  }

  public void setFar(double far) {
    mFar = far;
  }

  public double[] getMatrix() {
    return mMatrix;
  }

  public void setMatrix(double[] mtx) {
    assert mtx == null || mtx.length == 16;
    mMatrix = mtx;
  }

  private double mFov;
  private double mNear;
  private double mFar;
  private double[] mMatrix = MatrixHelper.createMatrix();

  @JsonIgnore
  private int mId;
}
