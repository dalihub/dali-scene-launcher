package com.samsung.dali.modelconverter.data.document.property;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collection;

import com.samsung.dali.modelconverter.data.document.BufferRef;
import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.MatrixHelper;

public class Property {

  public enum Type {
    Id, String, Integer, Number, Transform, BufferRef, Color
  }

  /*
   * Implementation of this interface signifies a class that may present
   * properties to a user.
   */
  public interface IReceiver {
    public abstract void resetProperties();

    public abstract void register(String name, Property property);
  }

  /*
   * Responsible for registering properties on a Property.Receiver.
   */
  public interface IProvider {
    public abstract void provideProperties(Document context, IReceiver receiver);
  }

  /*
   * Its responsibility is to provide a collection of values that are accepted for
   * the property.
   */
  public interface IValueProvider {
    public abstract Collection<?> getValues();
  }

  /*
   * Converts values between their internal format and its presentation. E.g.
   * matrix to translation/scale/rotation and back etc.
   */
  public interface IValueProcessor {
    public abstract Object present(Object in);

    public abstract Object store(Object in);
  }

  /*
   * Performs the access of the field either directly or via a getter/setter
   */
  static abstract class Accessor {
    Accessor(Object o) {
      mObject = o;
    }

    public abstract Object get() throws IllegalArgumentException, IllegalAccessException, InvocationTargetException;

    public abstract void set(Object obj)
        throws IllegalArgumentException, IllegalAccessException, InvocationTargetException;

    protected Object mObject;
  }

  public Property(Object object, String fieldName, Type type, boolean isWritable)
      throws NoSuchFieldException, NoSuchMethodException {
    this(object, fieldName, type, isWritable, null, null);
  }

  public Property(Object object, String fieldName, Type type, boolean isWritable, IValueProvider valueProvider,
      IValueProcessor valueProcessor) throws NoSuchFieldException, NoSuchMethodException {
    this(object, fieldName, type, isWritable, valueProvider, valueProcessor, null);
  }

  public Property(Object object, String fieldName, Type type, boolean isWritable, IValueProvider valueProvider,
      IValueProcessor valueProcessor, Class<?> actualType) throws NoSuchFieldException, NoSuchMethodException {

    assert fieldName != null;
    assert !fieldName.isEmpty();
    assert Character.isUpperCase(fieldName.charAt(0));

    mType = type;
    mIsWritable = isWritable;

    mValueProvider = valueProvider;
    mValueProcessor = valueProcessor;

    Class<? extends Object> clazz = object.getClass();
    Field field = null;
    try {
      field = clazz.getDeclaredField("m" + fieldName);
    } catch (NoSuchFieldException e) {
      // Ignore this for now, we'll try getters / setters.
    }

    if (field != null && field.isAccessible()) {
      mAccessor = new DirectAccessor(object, field);
    } else {
      Method getter = clazz.getDeclaredMethod("get" + fieldName);

      if (actualType == null) {
        switch (type) {
        case Id:
        case Integer:
          actualType = Integer.class;
          break;
        case Number:
          actualType = Number.class;
          break;
        case String:
          actualType = String.class;
          break;
        case Transform:
          actualType = MatrixHelper.createMatrix().getClass();
          break;
        case BufferRef:
          actualType = BufferRef.class;
          break;
        case Color:
          actualType = Number[].class;
          break;
        }
      }

      Method setter = null;
      try {
        setter = clazz.getDeclaredMethod("set" + fieldName, actualType);
      } catch (NoSuchMethodException e) {
        // It's ok if a class fails to expose a setter, but only for a read-only
        // property.
        if (isWritable) {
          throw e;
        }
      }

      mAccessor = new GetterSetterAccessor(object, getter, setter);
    }

  }

  /*
   * Gets the value of the Property.
   */
  public Object get() throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
    Object data = mAccessor.get();
    if (mValueProcessor != null) {
      data = mValueProcessor.present(data);
    }
    return data;
  }

  /*
   * Sets the value of the Property.
   */
  public void set(Object value) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
    if (mValueProcessor != null) {
      value = mValueProcessor.store(value);
    }
    mAccessor.set(value);
  }

  /*
   * Get the type of the Property to help decide its presentation.
   */
  public Type getType() {
    return mType;
  }

  /*
   * Whether the property can be set, or read only.
   */
  public boolean isWritable() {
    return mIsWritable;
  }

  /*
   * The Property.Receiver may call this to get a list of values, should this be
   * the nature of the Property.
   */
  public Collection<?> getRange() {
    return mValueProvider != null ? mValueProvider.getValues() : null;
  }

  private Type mType;
  private boolean mIsWritable;
  private Accessor mAccessor;
  private IValueProvider mValueProvider;
  private IValueProcessor mValueProcessor;
}
