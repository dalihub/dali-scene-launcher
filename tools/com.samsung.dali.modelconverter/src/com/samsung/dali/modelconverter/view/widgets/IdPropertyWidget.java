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

import java.util.Collection;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;

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
    for (Object o : values) {
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
