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

import java.io.File;
import java.util.TreeMap;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
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
    fd_mTxtProjectName.right = new FormAttachment(100, -10);
    mTxtProjectName.setLayoutData(fd_mTxtProjectName);

    Label lbProjectId = new Label(area, SWT.NONE);
    FormData fd_lbProjectId = new FormData();
    fd_lbProjectId.top = new FormAttachment(lbProjectName, 16);
    fd_lbProjectId.left = new FormAttachment(0, 10);
    lbProjectId.setLayoutData(fd_lbProjectId);
    lbProjectId.setText("Project ID:");

    mTxtProjectId = new Text(area, SWT.BORDER);
    FormData fd_mTxtProjectId = new FormData();
    fd_mTxtProjectId.left = new FormAttachment(lbProjectId, 6);
    fd_mTxtProjectId.top = new FormAttachment(lbProjectId, -5, SWT.TOP);
    fd_mTxtProjectId.right = new FormAttachment(100, -10);
    mTxtProjectId.setLayoutData(fd_mTxtProjectId);

    Label lbProjectPath = new Label(area, SWT.NONE);
    FormData fd_lbProjectPath = new FormData();
    fd_lbProjectPath.top = new FormAttachment(lbProjectId, 16);
    fd_lbProjectPath.left = new FormAttachment(0, 10);
    lbProjectPath.setLayoutData(fd_lbProjectPath);
    lbProjectPath.setText("Project Directory:");

    mTxtProjectPath = new Text(area, SWT.BORDER);
    FormData fd_mTxtProjectPath = new FormData();
    fd_mTxtProjectPath.top = new FormAttachment(lbProjectPath, 8);
    fd_mTxtProjectPath.left = new FormAttachment(lbProjectId, -2, SWT.LEFT);
    mTxtProjectPath.setLayoutData(fd_mTxtProjectPath);

    Button btnBrowse = new Button(area, SWT.NONE);
    btnBrowse.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        DirectoryDialog dd = new DirectoryDialog(parent.getShell());
        dd.setFilterPath(mProjectPath);
        String path = dd.open();
        if (path != null) {
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

    mLbError = new Label(area, SWT.NONE);
    mLbError.setAlignment(SWT.RIGHT);
    FormData fd_lbProjectPathError = new FormData();
    fd_lbProjectPathError.right = new FormAttachment(btnBrowse, 0, SWT.RIGHT);
    fd_lbProjectPathError.top = new FormAttachment(mTxtProjectPath, 16);
    fd_lbProjectPathError.left = new FormAttachment(0, 10);
    mLbError.setLayoutData(fd_lbProjectPathError);

    mTxtProjectName.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String name = ((Text) e.getSource()).getText();
        if (name.isEmpty()) {
          mErrors.put(Item.Name, "Please enter project name.");
        } else if (!name.matches("[\\p{L}0-9\\-_ ]*")) {
          mErrors.put(Item.Name, "Project name contains invalid characters.");
        } else {
          mErrors.remove(Item.Name);
        }

        updateState();
      }
    });

    mTxtProjectId.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String id = ((Text) e.getSource()).getText();
        if (id.isEmpty()) {
          mErrors.put(Item.Id, "Please specify project ID.");
        } else if (!id.matches("[a-z_][a-z0-9_]*(\\.[a-z_][a-z0-9_]*)*")) {
          mErrors.put(Item.Id, "Project ID format is invalid.");
        } else {
          mErrors.remove(Item.Id);
        }

        updateState();
      }
    });

    mTxtProjectPath.addModifyListener(new ModifyListener() {

      @Override
      public void modifyText(ModifyEvent e) {
        String path = ((Text) e.getSource()).getText();
        if (path.isEmpty()) {
          mErrors.put(Item.Path, "Please set project path.");
        } else {
          File f = new File(path).getAbsoluteFile();
          if (!f.exists()) {
            mErrors.put(Item.Path, "Project path doesn't exist.");
          } else if (!f.isDirectory()) {
            mErrors.put(Item.Path, "Project path is not a directory.");
          } else if (f.list().length != 0) {
            mErrors.put(Item.Path, "Project directory must be empty.");
          } else if (!f.canWrite()) {
            mErrors.put(Item.Path, "Cannot write project directory.");
          } else {
            mErrors.remove(Item.Path);
          }
        }

        updateState();
      }
    });

    mTxtProjectName.setText(mProjectName);
    mTxtProjectId.setText(mProjectId);
    mTxtProjectPath.setText(mProjectPath);

    return area;

  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    mBtnOk = createButton(parent, IDialogConstants.OK_ID, "Create", true);
    updateOkButtonState();

    createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
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

  private enum Item {
    Name, Id, Path
  };

  private void updateState() {
    if (mErrors.isEmpty()) {
      mLbError.setText("");
    } else {
      mLbError.setText(mErrors.firstEntry().getValue());
    }

    if (mBtnOk != null) {
      updateOkButtonState();
    }
  }

  private void updateOkButtonState() {
    mBtnOk.setEnabled(mErrors.isEmpty());
  }

  private Text mTxtProjectName;
  private Text mTxtProjectId;
  private Text mTxtProjectPath;
  private Label mLbError;

  private Button mBtnOk;

  private TreeMap<Item, String> mErrors = new TreeMap<Item, String>();

  private String mProjectName = "";
  private String mProjectId = "";
  private String mProjectPath = "";
}
