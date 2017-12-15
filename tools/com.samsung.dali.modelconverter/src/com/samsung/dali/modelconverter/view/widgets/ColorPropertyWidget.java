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

import java.text.DecimalFormat;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

/*
 * A widget for visualising the decomposition of a transform matrix into
 * translation, scale and pitch-yaw-roll rotation.
 * TODO: editing capabilities.
 */
public class ColorPropertyWidget extends PropertyWidgetBase {
  public ColorPropertyWidget(Composite parent, int style) {
    super(parent, style);

    Composite c = new Composite(parent, style);
    c.setLayout(new GridLayout(8, true));

    Label lblR = new Label(c, SWT.NONE);
    lblR.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
    lblR.setText("R");

    mR = new Text(c, SWT.BORDER);
    GridData gd_tx = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
    gd_tx.widthHint = 25;
    mR.setLayoutData(gd_tx);

    Label lblG = new Label(c, SWT.NONE);
    lblG.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
    lblG.setText("G");

    mG = new Text(c, SWT.BORDER);
    mG.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Label lblB = new Label(c, SWT.NONE);
    lblB.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
    lblB.setText("B");

    mB = new Text(c, SWT.BORDER);
    mB.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Label lblA = new Label(c, SWT.NONE);
    lblA.setLayoutData(new GridData(SWT.RIGHT, SWT.CENTER, false, false, 1, 1));
    lblA.setText("A");

    mA = new Text(c, SWT.BORDER);
    mA.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
}

  public ColorPropertyWidget setValue(double[] color) {
    DecimalFormat df = new DecimalFormat("#0.####");
    mR.setText(df.format(color[0]));
    mG.setText(df.format(color[1]));
    mB.setText(df.format(color[2]));
    mA.setText(df.format(color[3]));

    return this;
  }

  public ColorPropertyWidget setWritable(boolean isWritable) {
    mR.setEnabled(isWritable);
    mG.setEnabled(isWritable);
    mB.setEnabled(isWritable);
    mA.setEnabled(isWritable);
    return this;
  }

  private Text mR;
  private Text mG;
  private Text mB;
  private Text mA;
}
