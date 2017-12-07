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
import com.samsung.dali.modelconverter.data.document.property.Property;
import com.samsung.dali.modelconverter.data.document.property.Property.IReceiver;
import com.samsung.dali.modelconverter.view.parts.NodePropertiesPart;
import com.samsung.dali.modelconverter.view.widgets.IdPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TextPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TransformPropertyWidget;

public class NodePropertyReceiver implements IReceiver {

  NodePropertyReceiver(NodePropertiesPart part) {
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
      case Integer:
        // TODO: enable editing
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
          .setValue(Integer.toString((Integer) property.get())).setName(name);
        break;
      case Number:
        // TODO: enable editing
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
          .setValue(Double.toString((Double) property.get())).setName(name);
        break;
      case String:
        // TODO: enable editing
        new TextPropertyWidget(mPart.getComposite(), style).setWritable(false)
            .setValue((String) property.get()).setName(name);
        break;
      case Transform:
        double[] value = (double[]) property.get();
        double[] t = MatrixHelper.getTranslation(value);
        double[] s = MatrixHelper.getScale(value);
        double[] r = MatrixHelper.getRotation(value);
        // TODO: enable editing
        new TransformPropertyWidget(mPart.getComposite(), style).setValue(t, s, r).setWritable(false).setName(name);
        break;
      case Id:
        // TODO: enable editing
        new IdPropertyWidget(mPart.getComposite(), style).setWritable(false).setRange(values)
            .setSelection((Integer) property.get()).setName(name);
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

  NodePropertiesPart mPart;
}
