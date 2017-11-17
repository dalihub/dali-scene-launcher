package com.samsung.dali.modelConverter.model.document.uniforms;

import com.samsung.dali.modelConverter.model.document.Uniform;

public class Vec4Uniform extends Uniform {

  public Vec4Uniform(Number[] numbers) {
    assert numbers.length == 4;
    value = numbers;
  }
  
  Number[] value;

  @Override
  public Object getValue() {

    return value;
  }
}
