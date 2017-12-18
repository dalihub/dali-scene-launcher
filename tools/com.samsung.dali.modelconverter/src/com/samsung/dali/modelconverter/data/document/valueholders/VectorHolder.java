package com.samsung.dali.modelconverter.data.document.valueholders;

import com.samsung.dali.modelconverter.data.document.ValueHolder;

public class VectorHolder extends ValueHolder {

  public static final int MAX_SUPPORTED_ELEMENTS = 4;

  public VectorHolder(Number[] numbers) {
    assert numbers.length <= MAX_SUPPORTED_ELEMENTS;
    mValue = numbers;
  }

  @Override
  public Object get() {

    return mValue;
  }

  private Number[] mValue;

}
