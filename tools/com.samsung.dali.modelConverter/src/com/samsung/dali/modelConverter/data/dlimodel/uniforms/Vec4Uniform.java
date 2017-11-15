package com.samsung.dali.modelConverter.data.dlimodel.uniforms;

import com.samsung.dali.modelConverter.data.dlimodel.Uniform;

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
