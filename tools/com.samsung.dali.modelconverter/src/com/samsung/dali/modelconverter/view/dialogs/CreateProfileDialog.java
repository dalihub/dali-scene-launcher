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
package com.samsung.dali.modelconverter.view.dialogs;

import java.io.File;
import java.util.Collection;
import java.util.TreeMap;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelconverter.view.widgets.PathSelectionWidget;

public class CreateProfileDialog extends Dialog {

  public CreateProfileDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.APPLICATION_MODAL | SWT.TITLE | SWT.RESIZE);
  }

  @Override
  protected Control createDialogArea(Composite parent) {

    Composite area = (Composite) super.createDialogArea(parent);
    parent.getShell().setMinimumSize(460, 300);
    area.setLayout(new FormLayout());

    Label lblAlias = new Label(area, SWT.NONE);
    FormData fd_lblAlias = new FormData();
    fd_lblAlias.top = new FormAttachment(0, 10);
    fd_lblAlias.left = new FormAttachment(0, 10);
    lblAlias.setLayoutData(fd_lblAlias);
    lblAlias.setText("Profile Name:");

    mTxtAlias = new Text(area, SWT.BORDER);
    FormData fd_mTxtAlias = new FormData();
    fd_mTxtAlias.left = new FormAttachment(lblAlias, 6);
    fd_mTxtAlias.top = new FormAttachment(lblAlias, 0, SWT.TOP);
    fd_mTxtAlias.right = new FormAttachment(100, -10);
    mTxtAlias.setLayoutData(fd_mTxtAlias);
    mTxtAlias.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String value = ((Text) e.getSource()).getText();
        if (value.isEmpty()) {
          mErrors.put(Item.Name, "Please enter a name for the profile.");
        }
        else if (!value.matches("[\\p{L}0-9\\-\\.,_ ]*")) {
          mErrors.put(Item.Name, "Alias contains invalid characters.");
        }
        else if (mExistingProfiles.contains(value)) {
          mErrors.put(Item.Name, "Profile name already taken.");
        }
        else {
          mErrors.remove(Item.Name);
        }

        updateState();
      }
    });

    SelectionListener certificateOptionsListener = new SelectionListener() {

      @Override
      public void widgetSelected(SelectionEvent e) {
        mIsCreatingCertificate = mRbCreateCertificate.getSelection();
        mTxtCountry.setEnabled(mIsCreatingCertificate);
        mTxtCity.setEnabled(mIsCreatingCertificate);
        mTxtOrganization.setEnabled(mIsCreatingCertificate);
        mTxtEmail.setEnabled(mIsCreatingCertificate);

        mPswCertificate.getText().setText(mPswCertificate.getText().getText());

        if (mIsCreatingCertificate) {
          updateNewCertificateControls();
        }
        else {
          updateNewCertificateValues();
          mErrors.remove(Item.Country);
          mErrors.remove(Item.City);
          mErrors.remove(Item.Organization);
          mErrors.remove(Item.Email);
        }

        updateState();
      }

      @Override
      public void widgetDefaultSelected(SelectionEvent e) {
        // TODO Auto-generated method stub
      }

    };

    mRbUseExistingCertificate = new Button(area, SWT.RADIO);
    FormData fd_rbUseExistingCertificate = new FormData();
    fd_rbUseExistingCertificate.top = new FormAttachment(lblAlias, 16);
    fd_rbUseExistingCertificate.left = new FormAttachment(lblAlias, 0, SWT.LEFT);
    mRbUseExistingCertificate.setLayoutData(fd_rbUseExistingCertificate);
    mRbUseExistingCertificate.setText("Use existing certificate");
    mRbUseExistingCertificate.addSelectionListener(certificateOptionsListener);

    mRbCreateCertificate = new Button(area, SWT.RADIO);
    FormData fd_rbCreateCertificate = new FormData();
    fd_rbCreateCertificate.left = new FormAttachment(mRbUseExistingCertificate, 6);
    fd_rbCreateCertificate.top = new FormAttachment(mRbUseExistingCertificate, 0, SWT.TOP);
    mRbCreateCertificate.setLayoutData(fd_rbCreateCertificate);
    mRbCreateCertificate.setText("Create certificate");
    mRbCreateCertificate.addSelectionListener(certificateOptionsListener);

    mPswCertificate = new PathSelectionWidget(area, SWT.FILL, "Certificate location");
    FormData fd_mPswCertificate = new FormData();
    fd_mPswCertificate.top = new FormAttachment(mRbUseExistingCertificate, 6);
    fd_mPswCertificate.left = new FormAttachment(mRbUseExistingCertificate, 0, SWT.LEFT);
    fd_mPswCertificate.right = new FormAttachment(mTxtAlias, 0, SWT.RIGHT);
    mPswCertificate.setLayoutData(fd_mPswCertificate);
    mPswCertificate.getText().addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String value = ((Text) e.getSource()).getText();
        if (isCreatingCertificate()) {
          if (value.isEmpty()) {
            mErrors.put(Item.Path, "Please enter a name for the certificate.");
          }
          else {
            mErrors.remove(Item.Path);
          }
        }
        else {
          if (value.isEmpty()) {
            mErrors.put(Item.Path, "Please specify the path to the certificate.");
          }
          else {
            if (!value.endsWith(".p12")) {
              value += ".p12";
            }

            File certFile = new File(value);
            if (certFile != null && !(certFile.exists() && certFile.canRead())) {
              mErrors.put(Item.Path, "Certificate not accessible.");
            }
            else if (certFile != null && !certFile.isFile()) {
              mErrors.put(Item.Path, "Certificate is a directory; a file is needed.");
            }
            else {
              mErrors.remove(Item.Path);
            }
          }
        }

        updateState();
      }
    });

    Label lblPassword = new Label(area, SWT.NONE);
    FormData fd_lblPassword = new FormData();
    fd_lblPassword.right = new FormAttachment(mTxtAlias);
    fd_lblPassword.top = new FormAttachment(mPswCertificate, 16);
    fd_lblPassword.left = new FormAttachment(mPswCertificate, 0, SWT.LEFT);
    lblPassword.setLayoutData(fd_lblPassword);
    lblPassword.setText("Password:");

    mTxtPassword = new Text(area, SWT.BORDER | SWT.PASSWORD);
    FormData fd_mTxtPassword = new FormData();
    fd_mTxtPassword.left = new FormAttachment(lblPassword, 6);
    fd_mTxtPassword.top = new FormAttachment(lblPassword, 0, SWT.TOP);
    fd_mTxtPassword.right = new FormAttachment(mPswCertificate, 0, SWT.RIGHT);
    mTxtPassword.setLayoutData(fd_mTxtPassword);
    mTxtPassword.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String value = ((Text) e.getSource()).getText();
        if (value.isEmpty()) {
          mErrors.put(Item.Password, "Please enter a password.");
        }
        else {
          mErrors.remove(Item.Password);
        }
        updateState();
      }
    });

    Label lblCountry = new Label(area, SWT.NONE);
    lblCountry.setText("Country:");
    FormData fd_lblCountry = new FormData();
    fd_lblCountry.left = new FormAttachment(lblPassword, 0, SWT.LEFT);
    fd_lblCountry.top = new FormAttachment(lblPassword, 16);
    lblCountry.setLayoutData(fd_lblCountry);

    mTxtCountry = new Text(area, SWT.BORDER);
    FormData fd_mTxtCountry = new FormData();
    fd_mTxtCountry.left = new FormAttachment(lblCountry, 6);
    fd_mTxtCountry.top = new FormAttachment(lblCountry, 0, SWT.TOP);
    fd_mTxtCountry.right = new FormAttachment(mTxtPassword, 0, SWT.RIGHT);
    mTxtCountry.setLayoutData(fd_mTxtCountry);
    mTxtCountry.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        Text country = ((Text) e.getSource());
        if (country.isEnabled()) {
          String value = country.getText();
          if (!value.isEmpty() && !value.matches("[a-zA-Z]{2}")) {
            mErrors.put(Item.Country, "Please enter ISO-3166-2 country code.");
          }
          else {
            mErrors.remove(Item.Country);
          }

          updateState();
        }
      }
    });

    Label lblCity = new Label(area, SWT.NONE);
    lblCity.setText("City:");
    FormData fd_lblCity = new FormData();
    fd_lblCity.left = new FormAttachment(lblCountry, 0, SWT.LEFT);
    fd_lblCity.top = new FormAttachment(lblCountry, 16);
    lblCity.setLayoutData(fd_lblCity);

    mTxtCity = new Text(area, SWT.BORDER);
    FormData fd_mTxtCity = new FormData();
    fd_mTxtCity.top = new FormAttachment(lblCity, 0, SWT.TOP);
    fd_mTxtCity.left = new FormAttachment(lblCity, 6);
    fd_mTxtCity.right = new FormAttachment(mTxtCountry, 0, SWT.RIGHT);
    mTxtCity.setLayoutData(fd_mTxtCity);
    mTxtCity.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        Text city = ((Text) e.getSource());
        if (city.isEnabled()) {
          String value = city.getText();
          if (!value.matches("[\\p{L}0-9\\-\\.,_ ]*")) {
            mErrors.put(Item.City, "City contains invalid characters.");
          }
          else {
            mErrors.remove(Item.City);
          }

          updateState();
        }
      }
    });

    Label lblOrganization = new Label(area, SWT.NONE);
    lblOrganization.setText("Organization:");
    FormData fd_lblOrganization = new FormData();
    fd_lblOrganization.left = new FormAttachment(lblCity, 0, SWT.LEFT);
    fd_lblOrganization.top = new FormAttachment(lblCity, 16);
    lblOrganization.setLayoutData(fd_lblOrganization);

    mTxtOrganization = new Text(area, SWT.BORDER);
    FormData fd_mTxtOrganization = new FormData();
    fd_mTxtOrganization.left = new FormAttachment(lblOrganization, 6);
    fd_mTxtOrganization.top = new FormAttachment(lblOrganization, 0, SWT.TOP);
    fd_mTxtOrganization.right = new FormAttachment(mTxtCity, 0, SWT.RIGHT);
    mTxtOrganization.setLayoutData(fd_mTxtOrganization);
    mTxtOrganization.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        Text org = ((Text) e.getSource());
        if (org.isEnabled()) {
          String value = org.getText();
          if (!value.matches("[\\p{L}0-9\\-\\.,_ ]*")) {
            mErrors.put(Item.Organization, "Organization name contains invalid characters.");
          }
          else {
            mErrors.remove(Item.Organization);
          }

          updateState();
        }
      }
    });

    Label lblEmail = new Label(area, SWT.NONE);
    lblEmail.setText("E-mail:");
    FormData fd_lblEmail = new FormData();
    fd_lblEmail.left = new FormAttachment(lblOrganization, 0, SWT.LEFT);
    fd_lblEmail.top = new FormAttachment(lblOrganization, 16);
    lblEmail.setLayoutData(fd_lblEmail);

    mTxtEmail = new Text(area, SWT.BORDER);
    FormData fd_mTxtEmail = new FormData();
    fd_mTxtEmail.left = new FormAttachment(lblEmail, 6);
    fd_mTxtEmail.top = new FormAttachment(lblEmail, 0, SWT.TOP);
    fd_mTxtEmail.right = new FormAttachment(mTxtOrganization, 0, SWT.RIGHT);
    mTxtEmail.setLayoutData(fd_mTxtEmail);
    mTxtEmail.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        Text email = ((Text) e.getSource());
        if (email.isEnabled()) {
          String value = email.getText();
          if (value.length() > 254) {
            mErrors.put(Item.Email, "E-mail address too long.");
          }
          else if (!value.isEmpty() && !value.matches(
              "(?:[a-zA-Z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-zA-Z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])")) {
            mErrors.put(Item.Email, "E-mail address is invalid format.");
          }
          else {
            mErrors.remove(Item.Email);
          }

          updateState();
        }
      }
    });

    mLblError = new Label(area, SWT.NONE);
    mLblError.setAlignment(SWT.RIGHT);
    FormData fd_mLblError = new FormData();
    fd_mLblError.top = new FormAttachment(lblEmail, 16);
    fd_mLblError.left = new FormAttachment(lblEmail, 0, SWT.LEFT);
    fd_mLblError.right = new FormAttachment(mTxtEmail, 0, SWT.RIGHT);
    mLblError.setLayoutData(fd_mLblError);

    mIsCreatingCertificate = mRbCreateCertificate.getSelection();
    mTxtAlias.setText(mAlias);
    mPswCertificate.getText().setText(mCertificatePath);
    mTxtPassword.setText(mPassword);
    updateNewCertificateControls();

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Create Profile");
  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);

    mBtnOk = createButton(parent, IDialogConstants.OK_ID, "Create", true);
    updateOkButtonState();
  }

  @Override
  protected void okPressed() {
    mAlias = mTxtAlias.getText();
    mPassword = mTxtPassword.getText();

    updateNewCertificateValues();
    super.okPressed();
  }

  public void SetExistingProfiles(Collection<String> profiles) {
    mExistingProfiles = profiles;
  }

  public boolean isCreatingCertificate() {
    return mIsCreatingCertificate;
  }

  public String getAlias() {
    return mAlias;
  }

  public String getCertificatePath() {
    return mCertificatePath;
  }

  public String getPassword() {
    return mPassword;
  }

  public String getCountry() {
    return mCountry;
  }

  public String getCity() {
    return mCity;
  }

  public String getOrganization() {
    return mOrganization;
  }

  public String getEmail() {
    return mEmail;
  }

  protected void updateState() {
    if (mErrors.isEmpty()) {
      mLblError.setText("");
    }
    else {
      mLblError.setText(mErrors.firstEntry().getValue());
    }

    if (mBtnOk != null) {
      updateOkButtonState();
    }
  }

  private void updateNewCertificateControls() {
    mTxtCountry.setText(mCountry);
    mTxtCity.setText(mCity);
    mTxtOrganization.setText(mOrganization);
    mTxtEmail.setText(mEmail);
  }

  private void updateNewCertificateValues() {
    mCertificatePath = mPswCertificate.getText().getText();
    mCountry = mTxtCountry.getText().toUpperCase();
    mCity = mTxtCity.getText();
    mOrganization = mTxtOrganization.getText();
    mEmail = mTxtEmail.getText();
  }

  private void updateOkButtonState() {
    mBtnOk.setEnabled(mErrors.isEmpty());
  }

  private enum Item {
    Name, Path, Password, Country, City, Organization, Email
  };

  private Text mTxtAlias;
  private PathSelectionWidget mPswCertificate;
  private Text mTxtPassword;

  private Text mTxtCountry;
  private Text mTxtCity;
  private Text mTxtOrganization;
  private Text mTxtEmail;

  private Label mLblError;

  private Button mBtnOk;

  private Collection<String> mExistingProfiles;

  private String mAlias = "";
  private String mCertificatePath = "";
  private String mPassword = "";
  private boolean mIsCreatingCertificate;
  private String mCountry = "";
  private String mCity = "";
  private String mOrganization = "";
  private String mEmail = "";

  private TreeMap<Item, String> mErrors = new TreeMap<Item, String>();
  private Button mRbUseExistingCertificate;
  private Button mRbCreateCertificate;
}
