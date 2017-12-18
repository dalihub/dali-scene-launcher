package com.samsung.dali.modelconverter.data.document.valueholders;

import com.samsung.dali.modelconverter.data.document.ValueHolder;

public class IntHolder extends ValueHolder {

  public IntHolder(Integer i) {
    mValue = i;
  }

  @Override
  public Object get() {

    return mValue;
  }

  private Integer mValue;
}
