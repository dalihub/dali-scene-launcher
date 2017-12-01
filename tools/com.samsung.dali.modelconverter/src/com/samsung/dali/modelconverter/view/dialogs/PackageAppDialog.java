package com.samsung.dali.modelconverter.view.dialogs;

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

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class PackageAppDialog extends Dialog {

  public PackageAppDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.BORDER | SWT.RESIZE | SWT.TITLE);
  }

  @Override
  protected Control createDialogArea(Composite parent) {
    parent.getShell().setMinimumSize(450, 330);
    Composite area = (Composite) super.createDialogArea(parent);
    area.setLayout(new FormLayout());

    mLabelPath = new Label(area, SWT.BORDER);
    FormData fd_mLabelPath = new FormData();
    fd_mLabelPath.top = new FormAttachment(0, 33);
    fd_mLabelPath.left = new FormAttachment(0, 10);
    mLabelPath.setLayoutData(fd_mLabelPath);

    Label mLb1 = new Label(area, SWT.NONE);
    FormData fd_mLb1 = new FormData();
    fd_mLb1.top = new FormAttachment(0, 10);
    fd_mLb1.left = new FormAttachment(0, 10);
    mLb1.setLayoutData(fd_mLb1);
    mLb1.setText("Working directory");

    mChkPackage = new Button(area, SWT.CHECK);
    mChkPackage.setSelection(true);
    FormData fd_mChkPackage = new FormData();
    fd_mChkPackage.top = new FormAttachment(mLabelPath, 6);
    fd_mChkPackage.left = new FormAttachment(mLabelPath, 0, SWT.LEFT);
    mChkPackage.setLayoutData(fd_mChkPackage);
    mChkPackage.setText("Create package");

    mChkInstall = new Button(area, SWT.CHECK);
    mChkInstall.setSelection(true);
    FormData fd_mChkInstall = new FormData();
    fd_mChkInstall.top = new FormAttachment(mChkPackage, 6);
    fd_mChkInstall.left = new FormAttachment(mLabelPath, 0, SWT.LEFT);
    mChkInstall.setLayoutData(fd_mChkInstall);
    mChkInstall.setText("Install on device");

    mConsole = new StyledText(area, SWT.BORDER | SWT.MULTI);
    FormData fd_mConsole = new FormData();
    fd_mConsole.right = new FormAttachment(100, -10);
    fd_mConsole.bottom = new FormAttachment(100, -12);
    fd_mConsole.top = new FormAttachment(0, 120);
    fd_mConsole.left = new FormAttachment(0, 10);
    mConsole.setLayoutData(fd_mConsole);

    Label mLb2 = new Label(area, SWT.NONE);
    FormData fd_mLb2 = new FormData();
    fd_mLb2.top = new FormAttachment(mLabelPath, 6);
    fd_mLb2.left = new FormAttachment(mChkPackage, 75);
    mLb2.setLayoutData(fd_mLb2);
    mLb2.setText("Current profile:");

    Label mCurrentProfile = new Label(area, SWT.NONE);
    FormData fd_mCurrentProfile = new FormData();
    fd_mCurrentProfile.top = new FormAttachment(mLabelPath, 6);
    fd_mCurrentProfile.left = new FormAttachment(mLb2, 6);
    mCurrentProfile.setLayoutData(fd_mCurrentProfile);

    mLabelPath.setText(mPath);
    mChkPackage.setSelection(mBoolPackage);
    mChkInstall.setSelection(mBoolInstall);

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Install on Device");
  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    createButton(parent, IDialogConstants.OK_ID, "Install", true);
    createButton(parent, IDialogConstants.CANCEL_ID, "Close", false);
  }

  public String getPath() {
    return mPath;
  }

  public void setPath(String mPath) {
    this.mPath = mPath;
  }

  public StyledText getConsole() {
    return mConsole;
  }

  public boolean getChkPackage() {
    return mBoolPackage;
  }

  public void setChkPackage(boolean mChkPackage) {
    mBoolPackage = mChkPackage;
  }

  public boolean getChkInstall() {
    return mBoolInstall;
  }

  public void setChkInstall(boolean mChkInstall) {
    this.mBoolInstall = mChkInstall;
  }

  private Label mLabelPath;
  private StyledText mConsole;
  private Button mChkPackage;
  private Button mChkInstall;

  private String mPath;
  private boolean mBoolPackage;
  private boolean mBoolInstall;

}
