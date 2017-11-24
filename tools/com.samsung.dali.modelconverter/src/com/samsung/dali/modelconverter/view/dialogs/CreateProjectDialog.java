package com.samsung.dali.modelconverter.view.dialogs;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;

public class CreateProjectDialog extends Dialog {

  public CreateProjectDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.BORDER | SWT.CLOSE | SWT.RESIZE);

  }

  @Override
  protected Control createDialogArea(Composite parent) {
    Composite area = (Composite) super.createDialogArea(parent);
    area.setLayout(new FormLayout());

    Label mLb2 = new Label(area, SWT.NONE);
    FormData fd_mLb2 = new FormData();
    mLb2.setLayoutData(fd_mLb2);
    mLb2.setText("Project ID:");

    mTxtProjectID = new Text(area, SWT.BORDER);
    fd_mLb2.top = new FormAttachment(mTxtProjectID, 5, SWT.TOP);
    FormData fd_mTxtProjectID = new FormData();
    fd_mTxtProjectID.right = new FormAttachment(100, -10);
    fd_mTxtProjectID.left = new FormAttachment(mLb2, 6);
    mTxtProjectID.setLayoutData(fd_mTxtProjectID);

    Label mLb1 = new Label(area, SWT.NONE);
    fd_mLb2.left = new FormAttachment(mLb1, 0, SWT.LEFT);
    FormData fd_mLb1 = new FormData();
    fd_mLb1.top = new FormAttachment(0, 15);
    fd_mLb1.left = new FormAttachment(0, 10);
    mLb1.setLayoutData(fd_mLb1);
    mLb1.setText("Project name:");

    mTxtProjectName = new Text(area, SWT.BORDER);
    fd_mTxtProjectID.top = new FormAttachment(mTxtProjectName, 9);
    FormData fd_mTxtProjectName = new FormData();
    fd_mTxtProjectName.top = new FormAttachment(mLb1, -5, SWT.TOP);
    fd_mTxtProjectName.left = new FormAttachment(mLb1, 15);
    fd_mTxtProjectName.right = new FormAttachment(100, -96);
    mTxtProjectName.setLayoutData(fd_mTxtProjectName);

    Label mLb3 = new Label(area, SWT.NONE);
    FormData fd_mLb3 = new FormData();
    fd_mLb3.left = new FormAttachment(0, 10);
    mLb3.setLayoutData(fd_mLb3);
    mLb3.setText("Project Directory Path:");

    mTxtProjectPath = new Text(area, SWT.BORDER);
    fd_mLb3.bottom = new FormAttachment(mTxtProjectPath, -6);
    FormData fd_mTxtProjectPath = new FormData();
    fd_mTxtProjectPath.left = new FormAttachment(0, 10);
    fd_mTxtProjectPath.top = new FormAttachment(0, 128);
    mTxtProjectPath.setLayoutData(fd_mTxtProjectPath);

    mBtnBrowse = new Button(area, SWT.NONE);
    fd_mTxtProjectPath.right = new FormAttachment(mBtnBrowse, -6);
    FormData fd_mBtnBrowse = new FormData();
    fd_mBtnBrowse.top = new FormAttachment(0, 128);
    fd_mBtnBrowse.right = new FormAttachment(100, -10);
    fd_mBtnBrowse.left = new FormAttachment(0, 374);
    mBtnBrowse.setLayoutData(fd_mBtnBrowse);
    mBtnBrowse.setText("Browse");

    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    newShell.setText( "Create project" );
    super.configureShell(newShell);
  }

  @Override
  protected void okPressed() {

    super.okPressed();
  }

  private Button mBtnBrowse;
  private Text mTxtProjectName;
  private Text mTxtProjectID;
  private Text mTxtProjectPath;

}
