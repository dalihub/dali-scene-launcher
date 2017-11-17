package com.samsung.dali.modelConverter.model.document.uniforms;

import com.samsung.dali.modelConverter.model.document.Uniform;

public class IntUniform extends Uniform {

  public IntUniform(int intValue) {
    value = intValue;
  }

  public int value;

  @Override
  public Object getValue() {

    return new Integer(value);
  }
}
