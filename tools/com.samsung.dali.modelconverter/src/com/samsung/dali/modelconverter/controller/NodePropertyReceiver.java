package com.samsung.dali.modelconverter.controller;

import java.lang.reflect.InvocationTargetException;
import java.util.Collection;

import org.eclipse.swt.SWT;

import com.samsung.dali.modelconverter.data.document.MatrixHelper;
import com.samsung.dali.modelconverter.data.document.property.Property;
import com.samsung.dali.modelconverter.data.document.property.Property.Receiver;
import com.samsung.dali.modelconverter.view.parts.NodePropertiesPart;
import com.samsung.dali.modelconverter.view.widgets.IdPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TextPropertyWidget;
import com.samsung.dali.modelconverter.view.widgets.TransformPropertyWidget;

public class NodePropertyReceiver implements Receiver {

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
