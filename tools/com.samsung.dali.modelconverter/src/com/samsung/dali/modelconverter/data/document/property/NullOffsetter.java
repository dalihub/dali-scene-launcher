package com.samsung.dali.modelconverter.data.document.property;

import com.samsung.dali.modelconverter.data.document.property.Property.ValueProcessor;

public class NullOffsetter implements ValueProcessor {

  @Override
  public Object present(Object in) {
    return new Integer(((Integer) in).intValue() + 1);
  }

  @Override
  public Object store(Object in) {
    return new Integer(((Integer) in).intValue() - 1);
  }

}
