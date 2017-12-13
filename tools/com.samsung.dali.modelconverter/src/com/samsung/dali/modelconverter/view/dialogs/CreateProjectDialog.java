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
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class CreateProjectDialog extends Dialog {

  public CreateProjectDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.APPLICATION_MODAL | SWT.TITLE);
  }

  @Override
  protected Control createDialogArea(Composite parent) {
    parent.getShell().setMinimumSize(450, 250);
    Composite area = (Composite) super.createDialogArea(parent);
    area.setLayout(new FormLayout());

    Label lbProjectName = new Label(area, SWT.NONE);
    FormData fd_lbProjectName = new FormData();
    fd_lbProjectName.top = new FormAttachment(0, 15);
    fd_lbProjectName.left = new FormAttachment(0, 10);
    lbProjectName.setLayoutData(fd_lbProjectName);
    lbProjectName.setText("Project Name:");

    mTxtProjectName = new Text(area, SWT.BORDER);
    FormData fd_mTxtProjectName = new FormData();
    fd_mTxtProjectName.top = new FormAttachment(lbProjectName, -5, SWT.TOP);
    fd_mTxtProjectName.left = new FormAttachment(lbProjectName, 15);
    fd_mTxtProjectName.right = new FormAttachment(100, -96);
    mTxtProjectName.setLayoutData(fd_mTxtProjectName);

    Label lbProjectId = new Label(area, SWT.NONE);
    FormData fd_lbProjectId = new FormData();
    fd_lbProjectId.top = new FormAttachment(lbProjectName, 15);
    fd_lbProjectId.left = new FormAttachment(0, 10);
    lbProjectId.setLayoutData(fd_lbProjectId);
    lbProjectId.setText("Project ID:");

    mTxtProjectId = new Text(area, SWT.BORDER);
    FormData fd_mTxtProjectId = new FormData();
    fd_mTxtProjectId.left = new FormAttachment(lbProjectId, 6);
    fd_mTxtProjectId.top = new FormAttachment(lbProjectId, -5, SWT.TOP);
    fd_mTxtProjectId.right = new FormAttachment(100, -10);
    mTxtProjectId.setLayoutData(fd_mTxtProjectId);

    Label lbProjectDirectory = new Label(area, SWT.NONE);
    FormData fd_lbProjectDirectory = new FormData();
    fd_lbProjectDirectory.left = new FormAttachment(0, 10);
    fd_lbProjectDirectory.top = new FormAttachment(lbProjectId, 15);
    lbProjectDirectory.setLayoutData(fd_lbProjectDirectory);
    lbProjectDirectory.setText("Project Directory:");

    mTxtProjectPath = new Text(area, SWT.BORDER);
    FormData fd_mTxtProjectPath = new FormData();
    fd_mTxtProjectPath.top = new FormAttachment(lbProjectDirectory, 8);
    fd_mTxtProjectPath.left = new FormAttachment(lbProjectId, -2, SWT.LEFT);
    mTxtProjectPath.setLayoutData(fd_mTxtProjectPath);

    Button btnBrowse = new Button(area, SWT.NONE);
    btnBrowse.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        DirectoryDialog dd = new DirectoryDialog(parent.getShell());
        String path = dd.open();
        if (!path.isEmpty()) {
          mTxtProjectPath.setText(path);
        }
      }
    });
    fd_mTxtProjectPath.right = new FormAttachment(btnBrowse, -6);
    FormData fd_mBtnBrowse = new FormData();
    fd_mBtnBrowse.top = new FormAttachment(mTxtProjectPath, 0, SWT.TOP);
    fd_mBtnBrowse.right = new FormAttachment(100, -7);
    btnBrowse.setLayoutData(fd_mBtnBrowse);
    btnBrowse.setText("Browse");

    mTxtProjectName.setText(mProjectName);
    mTxtProjectId.setText(mProjectId);
    mTxtProjectPath.setText(mProjectPath);
    return area;
  }

  @Override
  protected void okPressed() {
    mProjectName = mTxtProjectName.getText();
    mProjectId = mTxtProjectId.getText();
    mProjectPath = mTxtProjectPath.getText();
    super.okPressed();
  }

  @Override
  protected void configureShell(Shell newShell) {
    newShell.setText("Create project");
    super.configureShell(newShell);
  }

  public String getProjectName() {
    return mProjectName;
  }

  public void setProjectName(String projectName) {
    mProjectName = projectName;
  }

  public String getProjectID() {
    return mProjectId;
  }

  public void setProjectID(String projectID) {
    mProjectId = projectID;
  }

  public String getProjectPath() {
    return mProjectPath;
  }

  public void setProjectPath(String projectPath) {
    mProjectPath = projectPath;
  }

  private Text mTxtProjectName;
  private Text mTxtProjectId;
  private Text mTxtProjectPath;

  private String mProjectName = "";
  private String mProjectId = "";
  private String mProjectPath = "";

}
