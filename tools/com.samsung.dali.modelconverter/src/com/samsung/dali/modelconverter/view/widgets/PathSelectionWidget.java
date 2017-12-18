package com.samsung.dali.modelconverter.view.widgets;

import java.io.File;

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
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Text;

public class PathSelectionWidget extends Composite {

  public PathSelectionWidget(Composite parent, int style, String defaultText) {
    super(parent, style);

    // create controls
    mTxtPath = new Text(this, SWT.BORDER);
    mTxtPath.setMessage(defaultText);

    mBtnSelectPath = new Button(this, SWT.BORDER);
    mBtnSelectPath.setText("Browse...");
    mBtnSelectPath.addSelectionListener(new SelectionListener() {

      @Override
      public void widgetSelected(SelectionEvent e) {
        if (mDirectoryMode) {
          DirectoryDialog dialog = new DirectoryDialog(parent.getShell(), dialogStyle);
          dialog.open();

          mTxtPath.setText(dialog.getFilterPath());
        }
        else {
          FileDialog dialog = new FileDialog(parent.getShell(), dialogStyle);
          dialog.setFilterPath(mTxtPath.getText());
          if (extensions != null) {
            dialog.setFilterExtensions(extensions);
          }
          dialog.open();

          mTxtPath.setText(dialog.getFilterPath() + File.separator + dialog.getFileName());
        }
      }

      @Override
      public void widgetDefaultSelected(SelectionEvent e) {
        // TODO Auto-generated method stub

      }

    });

    // Do layouts
    GridLayout layout = new GridLayout();
    layout.numColumns = 2;
    setLayout(layout);

    GridData textData = new GridData();
    textData.widthHint = 360;
    mTxtPath.setLayoutData(textData);
  }

  public void setDirectory(boolean isDir) {
    mDirectoryMode = isDir;
  }

  public void setFilterExtensions(String[] exts) {
    extensions = exts;
  }

  public void setDialogStyle(int style) {
    dialogStyle = style;
  }

  public Text getText() {
    return mTxtPath;
  }

  private Text mTxtPath;
  private Button mBtnSelectPath;

  private boolean mDirectoryMode = false;
  private String[] extensions;
  private int dialogStyle = SWT.OPEN;

}
