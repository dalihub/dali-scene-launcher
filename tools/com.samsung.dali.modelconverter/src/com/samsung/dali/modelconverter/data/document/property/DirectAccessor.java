package com.samsung.dali.modelconverter.data.document.property;

import java.lang.reflect.Field;

import com.samsung.dali.modelconverter.data.document.property.Property;

public class DirectAccessor extends Property.Accessor {

  DirectAccessor(Object o, Field f) {
    super(o);
    mField = f;
  }

  @Override
  public Object get() throws IllegalArgumentException, IllegalAccessException {
    return mField.get(mObject);
  }

  @Override
  public void set(Object value) throws IllegalArgumentException, IllegalAccessException {
    mField.set(mObject, value);
  }

  private Field mField;
}
