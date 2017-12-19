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
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class ManageProfilesDialog extends Dialog {

  public ManageProfilesDialog(Shell parentShell) {
    super(parentShell);

    setShellStyle(SWT.APPLICATION_MODAL | SWT.TITLE);
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
    lblNewLabel_1.setText("Select profile");

    mCmbProfiles = new Combo(area, SWT.NONE);
    FormData fd_mCmbProfiles = new FormData();
    fd_mCmbProfiles.right = new FormAttachment(0, 248);
    fd_mCmbProfiles.top = new FormAttachment(0, 27);
    fd_mCmbProfiles.left = new FormAttachment(0, 5);
    mCmbProfiles.setLayoutData(fd_mCmbProfiles);

    mBtnCreateProfile = new Button(area, SWT.NONE);
    setCreateProfileListener(mCreateProfileListener);

    FormData fd_mBtnCreateProfile = new FormData();
    fd_mBtnCreateProfile.right = new FormAttachment(100, -5);
    fd_mBtnCreateProfile.top = new FormAttachment(0, 27);
    mBtnCreateProfile.setLayoutData(fd_mBtnCreateProfile);
    mBtnCreateProfile.setText("Create profile");

    refreshProfiles();
    mCmbProfiles.select(mSelectedProfile);

    return area;
  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);

    mBtnOk = createButton(parent, IDialogConstants.OK_ID, "Select", true);
    updateOkButtonState();
  }

  @Override
  protected void okPressed() {
    mSelectedProfile = mCmbProfiles.getSelectionIndex();
    mResult = mCmbProfiles.getItem(mSelectedProfile);
    super.okPressed();
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Select profile");
  }

  public void setProfiles(List<String> profiles) {
    mProfiles = profiles;
    refreshProfiles();
  }

  public String getResult() {
    return mResult;
  }

  public void selectProfile(int i) {
    selectProfile(i, false);
  }

  public void selectProfile(int i, boolean updateUi) {
    mSelectedProfile = i;
    if(updateUi) {
      mCmbProfiles.select(i);
    }
  }

  public void setCreateProfileListener(SelectionListener listener) {
    if (mCreateProfileListener != null && mBtnCreateProfile != null) {
      mBtnCreateProfile.removeSelectionListener(mCreateProfileListener);
    }

    if (mBtnCreateProfile != null) {
      boolean isValid = listener != null;
      if (isValid) {
        mBtnCreateProfile.addSelectionListener(listener);
      }
      mBtnCreateProfile.setEnabled(isValid);
    }
    mCreateProfileListener = listener;
  }

  private void refreshProfiles() {
    if (mCmbProfiles != null) {
      mCmbProfiles.removeAll();
      for (String profile : mProfiles) {

        mCmbProfiles.add(profile);
      }
    }

    if (mBtnOk != null) {
      updateOkButtonState();
    }
  }

  private void updateOkButtonState() {
    mBtnOk.setEnabled(mProfiles != null && mProfiles.size() > 0);
  }

  private List<String> mProfiles;
  private int mSelectedProfile;

  private SelectionListener mCreateProfileListener = null;

  private Combo mCmbProfiles;
  private Button mBtnCreateProfile;
  private Button mBtnOk;

  private String mResult;
}
