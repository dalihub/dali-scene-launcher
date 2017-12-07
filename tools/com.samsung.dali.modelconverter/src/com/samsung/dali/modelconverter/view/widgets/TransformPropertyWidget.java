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
public class TransformPropertyWidget extends PropertyWidgetBase {
  public TransformPropertyWidget(Composite parent, int style) {
    super(parent, style);

    Composite c = new Composite(parent, style);
    c.setLayout(new GridLayout(3, true));

    Label lblTranslation = new Label(c, SWT.NONE);
    lblTranslation.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 3, 1));
    lblTranslation.setText("Translation");

    mTx = new Text(c, SWT.BORDER);
    GridData gd_tx = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
    gd_tx.widthHint = 25;
    mTx.setLayoutData(gd_tx);

    mTy = new Text(c, SWT.BORDER);
    mTy.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    mTz = new Text(c, SWT.BORDER);
    mTz.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Label lblScale = new Label(c, SWT.NONE);
    lblScale.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 3, 1));
    lblScale.setText("Scale");

    mSx = new Text(c, SWT.BORDER);
    mSx.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    mSy = new Text(c, SWT.BORDER);
    mSy.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    mSz = new Text(c, SWT.BORDER);
    mSz.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Label lblRotation = new Label(c, SWT.NONE);
    lblRotation.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 3, 1));
    lblRotation.setText("Rotation");

    mRx = new Text(c, SWT.BORDER);
    mRx.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    mRy = new Text(c, SWT.BORDER);
    mRy.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    mRz = new Text(c, SWT.BORDER);
    mRz.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
  }

  public TransformPropertyWidget setValue(double[] translation, double[] scale, double[] rotation) {
    DecimalFormat df = new DecimalFormat("#0.####");
    mTx.setText(df.format(translation[0]));
    mTy.setText(df.format(translation[1]));
    mTz.setText(df.format(translation[2]));

    mSx.setText(df.format(scale[0]));
    mSy.setText(df.format(scale[1]));
    mSz.setText(df.format(scale[2]));

    mRx.setText(df.format(rotation[0]));
    mRy.setText(df.format(rotation[1]));
    mRz.setText(df.format(rotation[2]));

    return this;
  }

  public TransformPropertyWidget setWritable(boolean isWritable) {
    mTx.setEnabled(isWritable);
    mTy.setEnabled(isWritable);
    mTz.setEnabled(isWritable);
    mSx.setEnabled(isWritable);
    mSy.setEnabled(isWritable);
    mSz.setEnabled(isWritable);
    mRx.setEnabled(isWritable);
    mRy.setEnabled(isWritable);
    mRz.setEnabled(isWritable);
    return this;
  }

  private Text mTx;
  private Text mTy;
  private Text mTz;
  private Text mSx;
  private Text mSy;
  private Text mSz;
  private Text mRx;
  private Text mRy;
  private Text mRz;
}
