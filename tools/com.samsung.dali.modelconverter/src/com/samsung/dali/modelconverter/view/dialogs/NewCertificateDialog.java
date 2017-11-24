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
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.GridData;

public class NewCertificateDialog extends Dialog {

  public NewCertificateDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.DIALOG_TRIM | SWT.RESIZE);
  }

  @Override
  protected Control createDialogArea(Composite parent) {

    Composite area = (Composite) super.createDialogArea(parent);
    parent.getShell().setMinimumSize(460, 300);
    area.setLayout(new GridLayout(2, false));

    Label mLb1 = new Label(area, SWT.NONE);
    mLb1.setText("Profile:");

    mTxtProfile = new Text(area, SWT.BORDER);
    mTxtProfile.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Label mLb2 = new Label(area, SWT.NONE);
    mLb2.setText("Password:");

    mTxtPassword = new Text(area, SWT.BORDER);
    mTxtPassword.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));

    Label mLb3 = new Label(area, SWT.NONE);
    mLb3.setText("Country:");

    mTxtCountry = new Text(area, SWT.BORDER);
    mTxtCountry.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));

    Label mLb4 = new Label(area, SWT.NONE);
    mLb4.setText("Organization:");

    mTxtOrganization = new Text(area, SWT.BORDER);
    mTxtOrganization.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));

    Label mLb5 = new Label(area, SWT.NONE);
    mLb5.setText("User name:");

    mTxtUsrName = new Text(area, SWT.BORDER);
    mTxtUsrName.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));

    Label mLb6 = new Label(area, SWT.NONE);
    mLb6.setText("email:");

    mTxtEmail = new Text(area, SWT.BORDER);
    mTxtEmail.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("New certificate");
  }

  public String getTxtProfile() {
    return mTxtProfile.getText();
  }

  public void setTxtProfile(String mTxtProfile) {
    this.mTxtProfile.setText(mTxtProfile);
  }

  public String getTxtPassword() {
    return mTxtPassword.getText();
  }

  public void setTxtPassword(String mTxtPassword) {
    this.mTxtPassword.setText(mTxtPassword);
  }

  public String getTxtCountry() {
    return mTxtCountry.getText();
  }

  public void setTxtCountry(String mTxtCountry) {
    this.mTxtCountry.setText(mTxtCountry);
  }

  public String getTxtOrganization() {
    return mTxtOrganization.getText();
  }

  public void setTxtOrganization(String mTxtOrganization) {
    this.mTxtOrganization.setText(mTxtOrganization);
  }

  public String getTxtUsrName() {
    return mTxtUsrName.getText();
  }

  public void setTxtUsrName(String mTxtUsrName) {
    this.mTxtUsrName.setText(mTxtUsrName);
  }

  public String getTxtEmail() {
    return mTxtEmail.getText();
  }

  public void setTxtEmail(String mTxtEmail) {
    this.mTxtEmail.setText(mTxtEmail);
  }

  private Text mTxtProfile;
  private Text mTxtPassword;
  private Text mTxtCountry;
  private Text mTxtOrganization;
  private Text mTxtUsrName;
  private Text mTxtEmail;
}
