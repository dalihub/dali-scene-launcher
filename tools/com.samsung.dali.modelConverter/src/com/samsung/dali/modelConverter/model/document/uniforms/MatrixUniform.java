package com.samsung.dali.modelConverter.model.document.uniforms;

import com.samsung.dali.modelConverter.model.document.Uniform;

public class MatrixUniform extends Uniform {

  public MatrixUniform(Number[] numbers) {
    assert numbers.length == 16;
    value = numbers;
  }
  
  Number[] value;

  @Override
  public Object getValue() {

    return value;
  }
}
