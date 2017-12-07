package com.samsung.dali.modelconverter.view.widgets;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

public class PropertyWidgetBase {

  public PropertyWidgetBase(Composite parent, int style) {

    mLabel = new Label(parent, SWT.NONE);
    mLabel.setAlignment(SWT.RIGHT);
    mLabel.setLayoutData(new GridData(SWT.FILL, SWT.TOP, false, false, 1, 1));
    mLabel.setText("SetMyLabel");
  }

  public PropertyWidgetBase setName(String name) {
    mLabel.setText(name);
    return this;
  }

  private Label mLabel;
}
