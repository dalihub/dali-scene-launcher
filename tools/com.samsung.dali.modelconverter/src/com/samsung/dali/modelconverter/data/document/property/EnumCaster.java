package com.samsung.dali.modelconverter.data.document.property;

import java.lang.reflect.ParameterizedType;

import com.samsung.dali.modelconverter.data.document.property.Property.IValueProcessor;

public class EnumCaster<T extends Enum<T>> implements IValueProcessor {

  @SuppressWarnings("unchecked")
  @Override
  public Object present(Object in) {
    if (in != null) {
      return ((T) in).ordinal();
    }
    return 0;
  }

  @Override
  public Object store(Object in) {
    if (in != null) {
      return tClass().getEnumConstants()[((Integer)in).intValue()];
    }
    return null;
  }

  @SuppressWarnings("unchecked")
  private Class<T> tClass() {
    ParameterizedType parameterizedType =
      (ParameterizedType) getClass().getGenericSuperclass();
    return (Class<T>) parameterizedType.getActualTypeArguments()[0];
  }
}
