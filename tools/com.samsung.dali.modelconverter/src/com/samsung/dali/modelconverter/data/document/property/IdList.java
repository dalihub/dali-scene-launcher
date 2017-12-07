package com.samsung.dali.modelconverter.data.document.property;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import com.samsung.dali.modelconverter.data.document.property.Property.ValueProvider;

public class IdList<T> implements ValueProvider {

  public IdList(List<T> values, boolean nullable) {
    mValues = new ArrayList<String>();
    if (nullable) {
      mValues.add("(none)");
    }
    for (Object o : values) {
      mValues.add(o.toString());
    }
  }

  public IdList(List<T> values) {
    this(values, false);
  }

  @Override
  public Collection<?> getValues() {
    return mValues;
  }

  List<String> mValues;
}
