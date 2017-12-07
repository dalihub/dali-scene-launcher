package com.samsung.dali.modelconverter.data.document.property;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class GetterSetterAccessor extends Property.Accessor {

  GetterSetterAccessor(Object o, Method getter, Method setter) {
    super(o);
    mGetter = getter;
    mSetter = setter;
  }

  @Override
  public Object get() throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
    return mGetter.invoke(mObject);
  }

  @Override
  public void set(Object value) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
    mSetter.invoke(mObject, value);
  }

  Method mGetter;
  Method mSetter;
}
