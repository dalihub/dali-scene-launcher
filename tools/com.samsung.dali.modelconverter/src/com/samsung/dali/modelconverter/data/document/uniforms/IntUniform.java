package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class IntUniform extends Uniform {

  public IntUniform(int intValue) {
    mValue = intValue;
  }

  @Override
  public Object getValue() {

    return new Integer(mValue);
  }

  private int mValue;

}
