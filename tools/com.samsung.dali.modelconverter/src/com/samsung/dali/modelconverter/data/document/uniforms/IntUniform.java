package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class IntUniform extends Uniform {

  public IntUniform(Integer i) {
    mValue = i;
  }

  @Override
  public Object getValue() {

    return mValue;
  }

  private Integer mValue;
}
