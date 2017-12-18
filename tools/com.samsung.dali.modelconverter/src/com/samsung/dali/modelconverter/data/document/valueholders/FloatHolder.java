package com.samsung.dali.modelconverter.data.document.valueholders;

import com.samsung.dali.modelconverter.data.document.ValueHolder;

public class FloatHolder extends ValueHolder {

  public FloatHolder(Number i) {
    mValue = i;
  }

  @Override
  public Object get() {

    return mValue;
  }

  private Number mValue;
}
