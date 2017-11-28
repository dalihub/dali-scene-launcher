package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class MatrixUniform extends Uniform {

  public MatrixUniform(Number[] numbers) {
    assert numbers.length == 16;
    mValue = numbers;
  }

  @Override
  public Object getValue() {

    return mValue;
  }

  private Number[] mValue;

}
