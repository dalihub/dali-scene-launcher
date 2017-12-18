package com.samsung.dali.modelconverter.data.document.valueholders;

import com.samsung.dali.modelconverter.data.document.ValueHolder;

public class MatrixHolder extends ValueHolder {

  public MatrixHolder(Number[] numbers) {
    assert numbers.length == 9 || numbers.length == 16;
    mValue = numbers;
  }

  @Override
  public Object get() {

    return mValue;
  }

  private Number[] mValue;

}
