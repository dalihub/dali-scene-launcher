package com.samsung.dali.modelconverter.view.widgets;

import java.util.Collection;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.layout.GridData;

public class IdPropertyWidget extends PropertyWidgetBase {

  public IdPropertyWidget(Composite parent, int style) {
    super(parent, style);

    mOptions = new Combo(parent, SWT.BORDER | SWT.V_SCROLL);
    GridData gd_mOptions = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
    gd_mOptions.widthHint = 240;
    mOptions.setLayoutData(gd_mOptions);
  }

  public IdPropertyWidget setRange(Collection<?> values) {
    mOptions.removeAll();
    for(Object o: values) {
      mOptions.add(o.toString());
    }
    return this;
  }

  public IdPropertyWidget setWritable(boolean isWritable) {
    mOptions.setEnabled(isWritable);
    return this;
  }
  
  public IdPropertyWidget setSelection(int i) {
    mOptions.select(i);
    mOptions.update();
    return this;
  }

  private Combo mOptions;
}
