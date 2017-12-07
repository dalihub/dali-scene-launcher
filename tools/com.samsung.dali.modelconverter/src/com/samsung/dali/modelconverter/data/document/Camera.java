package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class Camera implements Property.Provider {

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
  public void provideProperties(Document context, Property.Receiver receiver) {
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
