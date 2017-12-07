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
