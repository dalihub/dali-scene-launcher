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
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class DeployAppDialog extends Dialog {

  public DeployAppDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.BORDER | SWT.RESIZE | SWT.TITLE);
  }

  @Override
  protected Control createDialogArea(Composite parent) {
    parent.getShell().setMinimumSize(450, 330);
    Composite area = (Composite) super.createDialogArea(parent);
    area.setLayout(new FormLayout());

    Label lbWorkingDirectory = new Label(area, SWT.NONE);
    FormData fd_mLbWorkingDirectory = new FormData();
    fd_mLbWorkingDirectory.top = new FormAttachment(0, 10);
    fd_mLbWorkingDirectory.left = new FormAttachment(0, 10);
    lbWorkingDirectory.setLayoutData(fd_mLbWorkingDirectory);
    lbWorkingDirectory.setText("Working directory");

    mTxtWorkingDirectoryValue = new Text(area, SWT.NONE);
    mTxtWorkingDirectoryValue.setEnabled(false);
    mTxtWorkingDirectoryValue.setEditable(false);
    FormData fd_mTxtWorkingDirectoryValue = new FormData();
    fd_mTxtWorkingDirectoryValue.top = new FormAttachment(lbWorkingDirectory, 6);
    fd_mTxtWorkingDirectoryValue.left = new FormAttachment(0, 10);
    mTxtWorkingDirectoryValue.setLayoutData(fd_mTxtWorkingDirectoryValue);

    Label lbCurrentProfile = new Label(area, SWT.NONE);
    FormData fd_mLbCurrentProfile = new FormData();
    fd_mLbCurrentProfile.top = new FormAttachment(mTxtWorkingDirectoryValue, 10);
    fd_mLbCurrentProfile.left = new FormAttachment(0, 10);
    lbCurrentProfile.setLayoutData(fd_mLbCurrentProfile);
    lbCurrentProfile.setText("Current profile:");

    mTxtCurrentProfileValue = new Text(area, SWT.NONE);
    mTxtCurrentProfileValue.setEnabled(false);
    mTxtCurrentProfileValue.setEditable(false);
    FormData fd_mTxtCurrentProfileValue = new FormData();
    fd_mTxtCurrentProfileValue.top = new FormAttachment(lbCurrentProfile, 6);
    fd_mTxtCurrentProfileValue.left = new FormAttachment(0, 10);
    mTxtCurrentProfileValue.setLayoutData(fd_mTxtCurrentProfileValue);

    mChkPackage = new Button(area, SWT.CHECK);
    mChkPackage.setSelection(true);
    FormData fd_mChkPackage = new FormData();
    fd_mChkPackage.top = new FormAttachment(mTxtCurrentProfileValue, 10);
    fd_mChkPackage.left = new FormAttachment(lbWorkingDirectory, 0, SWT.LEFT);
    mChkPackage.setLayoutData(fd_mChkPackage);
    mChkPackage.setText("Create package");

    mChkInstall = new Button(area, SWT.CHECK);
    mChkInstall.setSelection(false);
    FormData fd_mChkInstall = new FormData();
    fd_mChkInstall.top = new FormAttachment(mChkPackage, 6);
    fd_mChkInstall.left = new FormAttachment(lbWorkingDirectory, 0, SWT.LEFT);
    mChkInstall.setLayoutData(fd_mChkInstall);
    mChkInstall.setText("Install on device");

    mTxtWorkingDirectoryValue.setText(mPath);
    mTxtCurrentProfileValue.setText(mCurrentProfile);

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Create Package");
  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    createButton(parent, IDialogConstants.OK_ID, "Install", true);
    createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
  }

  @Override
  protected void okPressed() {
    mPerformPackaging = mChkPackage.getSelection();
    mPerformInstallation = mChkInstall.getSelection();
    super.okPressed();
  }

  public void setPath(String path) {
    mPath = path;
  }

  public void setCurrentProfile(String profileName) {
    mCurrentProfile = profileName;
  }

  public boolean isPerformPackagingRequested() {
    return mPerformPackaging;
  }

  public boolean isPerformInstallationRequested() {
    return mPerformInstallation;
  }

  private Text mTxtWorkingDirectoryValue;
  private Text mTxtCurrentProfileValue;

  private Button mChkPackage;
  private Button mChkInstall;

  private String mPath = "";
  private String mCurrentProfile = "";

  private boolean mPerformPackaging;
  private boolean mPerformInstallation;
}
