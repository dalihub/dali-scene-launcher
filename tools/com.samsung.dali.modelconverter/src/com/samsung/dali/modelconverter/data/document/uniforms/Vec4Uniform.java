package com.samsung.dali.modelconverter.data.document.uniforms;

import com.samsung.dali.modelconverter.data.document.Uniform;

public class Vec4Uniform extends Uniform {

  public Vec4Uniform(Number[] numbers) {
    assert numbers.length == 4;
    mValue = numbers;
  }

  @Override
  public Object getValue() {

    return mValue;
  }

  private Number[] mValue;

}
