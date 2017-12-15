package com.samsung.dali.modelconverter.controller;

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

import java.lang.reflect.InvocationTargetException;
import java.util.Collection;

import org.eclipse.swt.SWT;

import com.samsung.dali.modelconverter.data.document.MatrixHelper;
import com.samsung.dali.modelconverter.data.document.BufferRef;
import com.samsung.dali.modelconverter.data.document.property.Property;
import com.samsung.dali.modelconverter.view.parts.PropertiesPart;
import com.samsung.dali.modelconverter.view.widgets.IdPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TextPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TransformPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.BufferRefPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.ColorPropertyWidget;

public class PropertiesPresenter implements Property.IReceiver {

  PropertiesPresenter(PropertiesPart part) {
    mPart = part;
  }

  @Override
  public void resetProperties() {
    mPart.resetProperties();
  }

  @Override
  public void register(String name, Property property) {
    int style = SWT.NONE;
    Collection<?> values = property.getRange();
    try {
      switch (property.getType()) {
      case Integer: {
        // TODO: enable editing
        String string = "";

        Object object = property.get();
        if( null != object ) {
          string = Integer.toString( (Integer)object );
        }
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
          .setValue( string ).setName(name);
        break;
      }
      case Number: {
        // TODO: enable editing
        String string = "";

        Object object = property.get();
        if( null != object ) {
          string = Double.toString( (Double)object );
        }
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
          .setValue( string ).setName(name);
        break;
      }
      case String: {
        // TODO: enable editing
        String string = "";
        Object object = property.get();
        if( null != object ) {
          string = (String)object;
        }
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
            .setValue( string ).setName(name);
        break;
      }
      case Transform: {
        double[] matrix = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

        Object object = property.get();

        if( null != object) {
          if( object instanceof double[] ) {
            matrix = (double[])object;
          }
          else if( object instanceof Number[] ) {
            Number[] numberArray = (Number[])object;

            for( int index = 0; index < numberArray.length; index++ ) {
              matrix[index] = (double)((int)(numberArray[index]));
            }
          }
        }

        double[] t = MatrixHelper.getTranslation(matrix);
        double[] s = MatrixHelper.getScale(matrix);
        double[] r = MatrixHelper.getRotation(matrix);
        // TODO: enable editing
        new TransformPropertyWidget(mPart.getComposite(), style).setValue(t, s, r).setWritable(false).setName(name);
        break;
      }
      case Id: {
        // TODO: enable editing
        new IdPropertyWidget(mPart.getComposite(), style).setWritable(false).setRange(values)
            .setSelection((Integer) property.get()).setName(name);
        break;
      }
      case BufferRef: {
        BufferRef bufferRef = (BufferRef)( property.get() );
        new BufferRefPropertyWidget(mPart.getComposite(), style).setValue(bufferRef.mByteOffset, bufferRef.mByteLength).setWritable(false).setName(name);
        break;
      }
      case Color: {
        double[] color = { 1.0, 1.0, 1.0, 1.0 };
        Object object = property.get();
        if( null != object ) {
          Number[] numberArray = (Number[])object;
          for( int index = 0; index < numberArray.length; index++ ) {
            Number number = numberArray[index];
            if( null != number ) {
              color[index] = (double)number;
            }
          }
        }
        new ColorPropertyWidget(mPart.getComposite(), style).setValue( color ).setWritable(false).setName(name);
        break;
      }
      default:
        break;
      }
    }
    catch (IllegalArgumentException | IllegalAccessException | InvocationTargetException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }

    mPart.getComposite().requestLayout();
  }

  PropertiesPart mPart;
}
