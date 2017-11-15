package com.samsung.dali.modelConverter.windows;

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

import com.samsung.dali.modelConverter.data.SettingsClass;

import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;

public class SelectCertificateDialog extends Dialog {
  private List<String> mProfiles;
  private Combo mCmbCertificate;

  public SelectCertificateDialog(Shell parentShell, List<String> profiles) {
    super(parentShell);
    mProfiles = profiles;
  }

  @Override
  protected Control createDialogArea(Composite parent) {
    Composite control = (Composite) super.createDialogArea(parent);
    parent.getShell().setMinimumSize(400, 200);
    control.setLayout(new FormLayout());

    Label lblNewLabel_1 = new Label(control, SWT.NONE);
    FormData fd_lblNewLabel_1 = new FormData();
    fd_lblNewLabel_1.top = new FormAttachment(0, 5);
    fd_lblNewLabel_1.left = new FormAttachment(0, 5);
    lblNewLabel_1.setLayoutData(fd_lblNewLabel_1);
    lblNewLabel_1.setText("Select certificate");

    Label lblNewLabel = new Label(control, SWT.NONE);
    FormData fd_lblNewLabel = new FormData();
    fd_lblNewLabel.right = new FormAttachment(0, 303);
    fd_lblNewLabel.top = new FormAttachment(0, 5);
    fd_lblNewLabel.left = new FormAttachment(0, 253);
    lblNewLabel.setLayoutData(fd_lblNewLabel);

    mCmbCertificate = new Combo(control, SWT.NONE);
    FormData fd_mCmbCertificate = new FormData();
    fd_mCmbCertificate.right = new FormAttachment(0, 248);
    fd_mCmbCertificate.top = new FormAttachment(0, 27);
    fd_mCmbCertificate.left = new FormAttachment(0, 5);
    mCmbCertificate.setLayoutData(fd_mCmbCertificate);
    mCmbCertificate.setItems( mProfiles.toArray( new String[0] ) );

    Button mbtnNewCert = new Button(control, SWT.NONE);
    FormData fd_mbtnNewCert = new FormData();
    fd_mbtnNewCert.right = new FormAttachment(100, -5);
    fd_mbtnNewCert.top = new FormAttachment(0, 27);
    mbtnNewCert.setLayoutData(fd_mbtnNewCert);
    mbtnNewCert.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        NewCertificateDialog newCert = new NewCertificateDialog(parent.getShell());
        newCert.open();

      }
    });
    mbtnNewCert.setText("Create certificate");

    String selprofile = SettingsClass.Get().GetString(SettingsClass.TIZEN_CURRENT_PROFILE);
    if( mProfiles.indexOf(selprofile) >= 0)
    {
      mCmbCertificate.setText(selprofile);
    }
    return control;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Select certificate");
  }

  @Override
  protected void okPressed() {
    String value = mCmbCertificate.getText();
    SettingsClass.Get().SetString(SettingsClass.TIZEN_CURRENT_PROFILE, value);
    super.okPressed();
  }


}
