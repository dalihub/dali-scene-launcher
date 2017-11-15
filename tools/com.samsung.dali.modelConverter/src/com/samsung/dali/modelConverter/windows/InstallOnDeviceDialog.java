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

import java.io.File;
import java.io.IOException;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.SettingsClass;
import com.samsung.dali.modelConverter.process.ILogText;
import com.samsung.dali.modelConverter.process.LoggingProcessRunner;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Cursor;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;

public class InstallOnDeviceDialog extends Dialog implements ILogText {
  private Text mPath;
  private StyledText mConsole;
  private Button mChkPackage;
  private Button mChkInstall;
  private Button mBtnBrowse;
  private Cursor mCursor;
  private Button mOkButton;
  private Button mCloseButton;

  public InstallOnDeviceDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.BORDER | SWT.RESIZE | SWT.TITLE);
  }


  @Override
  protected Control createDialogArea(Composite parent) {
    parent.getShell().setMinimumSize(450, 330);
    Composite area = (Composite) super.createDialogArea(parent);
    area.setLayout(new FormLayout());

    mPath = new Text(area, SWT.BORDER);
    FormData fd_mPath = new FormData();
    fd_mPath.top = new FormAttachment(0, 33);
    fd_mPath.left = new FormAttachment(0, 10);
    mPath.setLayoutData(fd_mPath);

    mBtnBrowse = new Button(area, SWT.NONE);
    mBtnBrowse.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        DirectoryDialog dialog = new DirectoryDialog(parent.getShell());
        dialog.open();
        GlobalData.Get().SetInstallPackagePath(dialog.getFilterPath());
        SettingsClass.Get().SetString( SettingsClass.TIZEN_CURRENT_WORKPATH, GlobalData.Get().GetInstallPackagePath() );
      }
    });
    fd_mPath.right = new FormAttachment(mBtnBrowse, -6);
    FormData fd_mBtnBrowse = new FormData();
    fd_mBtnBrowse.top = new FormAttachment(0, 33);
    fd_mBtnBrowse.right = new FormAttachment(100, -10);
    mBtnBrowse.setLayoutData(fd_mBtnBrowse);
    mBtnBrowse.setText("Browse");

    Label mlb1 = new Label(area, SWT.NONE);
    FormData fd_mlb1 = new FormData();
    fd_mlb1.top = new FormAttachment(0, 10);
    fd_mlb1.left = new FormAttachment(0, 10);
    mlb1.setLayoutData(fd_mlb1);
    mlb1.setText("Working directory");

    mChkPackage = new Button(area, SWT.CHECK);
    mChkPackage.setSelection(true);
    FormData fd_mChkPackage = new FormData();
    fd_mChkPackage.top = new FormAttachment(mPath, 6);
    fd_mChkPackage.left = new FormAttachment(mPath, 0, SWT.LEFT);
    mChkPackage.setLayoutData(fd_mChkPackage);
    mChkPackage.setText("Create package");

    mChkInstall = new Button(area, SWT.CHECK);
    mChkInstall.setSelection(true);
    FormData fd_mChkInstall = new FormData();
    fd_mChkInstall.top = new FormAttachment(mChkPackage, 6);
    fd_mChkInstall.left = new FormAttachment(mPath, 0, SWT.LEFT);
    mChkInstall.setLayoutData(fd_mChkInstall);
    mChkInstall.setText("Install on device");

    mPath.setText( GlobalData.Get().GetInstallPackagePath() );

    mConsole = new StyledText(area, SWT.BORDER | SWT.MULTI);
    FormData fd_mConsole = new FormData();
    fd_mConsole.right = new FormAttachment(100, -10);
    fd_mConsole.bottom = new FormAttachment(100, -12);
    fd_mConsole.top = new FormAttachment(0, 120);
    fd_mConsole.left = new FormAttachment(0, 10);
    mConsole.setLayoutData(fd_mConsole);

    Label lblNewLabel = new Label(area, SWT.NONE);
    FormData fd_lblNewLabel = new FormData();
    fd_lblNewLabel.top = new FormAttachment(mPath, 6);
    fd_lblNewLabel.left = new FormAttachment(mChkPackage, 75);
    lblNewLabel.setLayoutData(fd_lblNewLabel);
    lblNewLabel.setText("Current profile:");

    Label mCurrentProfile = new Label(area, SWT.NONE);
    FormData fd_mCurrentProfile = new FormData();
    fd_mCurrentProfile.top = new FormAttachment(mPath, 6);
    fd_mCurrentProfile.left = new FormAttachment(lblNewLabel, 6);
    mCurrentProfile.setLayoutData(fd_mCurrentProfile);
    mCurrentProfile.setText(GlobalData.Get().GetCurrentProfile());
    return area;
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell(newShell);
    newShell.setText("Install on Device");
  }


  @Override
  protected void okPressed() {

    try
    {
      String extension = "";

      LoggingProcessRunner lpr = LoggingProcessRunner.create( this );

      if( mChkPackage.getSelection() ) {
        if( GlobalData.Get().IsWindows() )
        {
          extension = ".bat";
        }
        lpr.addCommand( GlobalData.Get().GetTizenPath() + "tizen" + extension  + " package -t tpk -s " + GlobalData.Get().GetCurrentProfile()
            + " -- " + mPath.getText() + File.separator + "Release" + File.separator );
      }

      if( mChkInstall.getSelection() ) {
        if( GlobalData.Get().IsWindows() )
        {
          extension = ".exe";
        }
        lpr.addCommand(GlobalData.Get().GetSdbPath() + "sdb" + " install " +
            mPath.getText() + File.separator + "Release" + File.separator + "org.tizen.watch3d-1.0.0-arm.tpk" );
      }
      lpr.run();

    }
    catch (IOException e)
    {
      SettingsClass.CheckTizenCliPath( getShell(), true );
    }

  }



  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    mOkButton = createButton(parent, IDialogConstants.OK_ID, "Install", true );
    mCloseButton = createButton(parent, IDialogConstants.CANCEL_ID, "Close", false );
  }


  @Override
  public void SetBusy(boolean isBusy) {
    Display display = mConsole.getDisplay();
    if(!display.isDisposed())
    {
      display.asyncExec(new Runnable() {

        @Override
        public void run() {
          // update controls -- note that the busy cursor won't show over enabled text fields (which is most of the UI).
          boolean isActive = !isBusy;
          mBtnBrowse.setEnabled(isActive);
          mPath.setEnabled(isActive);
          mConsole.setEnabled(isActive);
          mOkButton.setEnabled(isActive);
          mCloseButton.setEnabled(isActive);

          int cursorType = isBusy ? SWT.CURSOR_WAIT : SWT.CURSOR_ARROW;
          Cursor cursor = new Cursor(display, cursorType);
          mConsole.getShell().setCursor(cursor);

          if(mCursor != null)
          {
            mCursor.dispose();
          }
          mCursor = cursor;
        }
      });
    }
  }


  @Override
  public void AppendText(String str, int color) {
    Display display = mConsole.getDisplay();
    if( !display.isDisposed() )
    {
      display.asyncExec(new Runnable() {
        public void run()
        {
          Color swtColor = null;
          if(color != SWT.NONE)
          {
            swtColor = display.getSystemColor(color);
          }

          StyleRange styleRange = null;
          if(swtColor != null)
          {
            styleRange = new StyleRange(mConsole.getText().length(), str.length(), swtColor, null);
          }
          mConsole.setText( mConsole.getText() + str );
          mConsole.setTopIndex( mConsole.getLineCount() );
          if( styleRange != null )
          {
            mConsole.setStyleRange( styleRange );
          }
        }
      });
    }

  }


  @Override
  public void ClearText() {
    Display display = mConsole.getDisplay();
    if(!display.isDisposed())
    {
      display.asyncExec(new Runnable() {
        @Override
        public void run() {
          mConsole.setText("");
        }
      });
    }

  }


}
