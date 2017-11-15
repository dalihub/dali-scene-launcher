package com.samsung.dali.modelConverter.data.dlimodel.uniforms;

import com.samsung.dali.modelConverter.data.dlimodel.Uniform;

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
