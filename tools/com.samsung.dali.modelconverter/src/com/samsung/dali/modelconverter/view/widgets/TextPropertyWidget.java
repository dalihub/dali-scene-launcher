package com.samsung.dali.modelconverter.view.widgets;

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

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

/*
 * A widget for a property whose value can be presented as a Text box.
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
