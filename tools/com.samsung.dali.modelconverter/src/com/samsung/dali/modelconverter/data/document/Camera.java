package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Camera {

  static public void resetCounter()
  {
    sCounter = 0;
  }
  
  public Camera()
  {
    ++sCounter;
    mId = sCounter;
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

  private static int sCounter = 0;

	@JsonProperty("fov")
	private double mFov;

	@JsonProperty("near")
	private double mNear;

	@JsonProperty("far")
	private double mFar;

	@JsonProperty("matrix")
	private double[] mMatrix = MatrixHelper.createMatrix();

	@JsonIgnore
	private int mId;
}
