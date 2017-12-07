package com.samsung.dali.modelconverter.view.widgets;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

/*
 * A widget for a property whose value can be presented as a text entry. 
 */
public class TextPropertyWidget extends PropertyWidgetBase {

  public TextPropertyWidget(Composite parent, int style) {
    super(parent, style);

    mText = new Text(parent, SWT.BORDER);
    GridData gd_mText = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
    gd_mText.widthHint = 200;
    mText.setLayoutData(gd_mText);
  }

  public TextPropertyWidget setWritable(boolean isWritable) {
    mText.setEnabled(isWritable);
    return this;
  }

  public TextPropertyWidget setValue(String value) {
    mText.setText(value);
    return this;
  }

  private Text mText;
}
