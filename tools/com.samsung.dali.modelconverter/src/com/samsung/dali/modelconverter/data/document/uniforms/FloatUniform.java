package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class FloatUniform extends Uniform {

  public FloatUniform(Number i) {
    mValue = i;
  }

  @Override
  public Object getValue() {

    return mValue;
  }

  private Number mValue;
}
