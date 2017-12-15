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
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

/*
 * A widget for visualising the decomposition of a transform matrix into
 * translation, scale and pitch-yaw-roll rotation.
 * TODO: editing capabilities.
 */
public class BufferRefPropertyWidget extends PropertyWidgetBase {
  public BufferRefPropertyWidget(Composite parent, int style) {
    super(parent, style);

    Composite c = new Composite(parent, style);
    c.setLayout(new GridLayout(4, true));

    Label lblOffset = new Label(c, SWT.NONE);
    lblOffset.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
    lblOffset.setText("Offset");

    mOffset = new Text(c, SWT.BORDER);
    GridData gd_tx = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
    gd_tx.widthHint = 25;
    mOffset.setLayoutData(gd_tx);

    Label lblLength = new Label(c, SWT.NONE);
    lblLength.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
    lblLength.setText("Length");

    mLength = new Text(c, SWT.BORDER);
    mLength.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
  }

  public BufferRefPropertyWidget setValue(long offset, long length) {
    String offsetStr = "" + offset;
    String lengthStr = "" + length;
    mOffset.setText( offsetStr );
    mLength.setText( lengthStr );

    return this;
  }

  public BufferRefPropertyWidget setWritable(boolean isWritable) {
    mOffset.setEnabled(isWritable);
    mLength.setEnabled(isWritable);
    return this;
  }

  private Text mOffset;
  private Text mLength;
}
