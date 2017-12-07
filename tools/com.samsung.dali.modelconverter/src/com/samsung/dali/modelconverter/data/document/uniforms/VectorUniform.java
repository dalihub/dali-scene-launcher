package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class VectorUniform extends Uniform {

  public static final int MAX_SUPPORTED_ELEMENTS = 4;
  
  public VectorUniform(Number[] numbers) {
    assert numbers.length <= MAX_SUPPORTED_ELEMENTS;
    mValue = numbers;
  }

  @Override
  public Object getValue() {

    return mValue;
  }

  private Number[] mValue;

}
