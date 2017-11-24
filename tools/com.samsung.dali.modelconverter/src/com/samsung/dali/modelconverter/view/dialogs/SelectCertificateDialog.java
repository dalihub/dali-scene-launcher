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

import java.util.List;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;


import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;

public class SelectCertificateDialog extends Dialog {



  public SelectCertificateDialog(Shell parentShell) {
    super(parentShell);
  }

  @Override
  protected Control createDialogArea(Composite parent) {
    Composite area = (Composite) super.createDialogArea(parent);
    parent.getShell().setMinimumSize(400, 200);
    area.setLayout(new FormLayout());

    Label lblNewLabel_1 = new Label(area, SWT.NONE);
    FormData fd_lblNewLabel_1 = new FormData();
    fd_lblNewLabel_1.top = new FormAttachment(0, 5);
    fd_lblNewLabel_1.left = new FormAttachment(0, 5);
    lblNewLabel_1.setLayoutData(fd_lblNewLabel_1);
    lblNewLabel_1.setText("Select certificate");

    mCmbCertificate = new Combo(area, SWT.NONE);
    FormData fd_mCmbCertificate = new FormData();
    fd_mCmbCertificate.right = new FormAttachment(0, 248);
    fd_mCmbCertificate.top = new FormAttachment(0, 27);
    fd_mCmbCertificate.left = new FormAttachment(0, 5);
    mCmbCertificate.setLayoutData(fd_mCmbCertificate);

    Button mBtnNewCert = new Button(area, SWT.NONE);
    FormData fd_mBtnNewCert = new FormData();
    fd_mBtnNewCert.right = new FormAttachment(100, -5);
    fd_mBtnNewCert.top = new FormAttachment(0, 27);
    mBtnNewCert.setLayoutData(fd_mBtnNewCert);
    mBtnNewCert.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {

      }
    });
    mBtnNewCert.setText("Create certificate");

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Select certificate");
  }

  @Override
  protected void okPressed() {
    super.okPressed();
  }

  public Combo getCmbCertificate() {
    return mCmbCertificate;
  }

  public void setCmbCertificate(Combo mCmbCertificate) {
    this.mCmbCertificate = mCmbCertificate;
  }

  private Combo mCmbCertificate;
}
