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
import java.io.UnsupportedEncodingException;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Text;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.Project;
import com.samsung.dali.modelConverter.process.NativeExporter;

import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.FileDialog;

public class ImportDialog extends Dialog {
  private Text mModelPath;
  private Text mOutPath;
  private Text mConsoleText;

  public ImportDialog(Shell parentShell) {
    super(parentShell);
    setShellStyle(SWT.DIALOG_TRIM | SWT.RESIZE);

  }

  @Override
  protected Control createDialogArea(Composite parent) {
    parent.getShell().setMinimumSize(450, 350);
    Composite control = (Composite) super.createDialogArea(parent);
    control.setLayout(new GridLayout(2, false));

    Label mLabel1 = new Label(control, SWT.NONE);
    mLabel1.setText("Select model format (DAE, OBJ, etc)");
    new Label(control, SWT.NONE);

    mModelPath = new Text(control, SWT.BORDER);
    mModelPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Button mbtnModelPath = new Button(control, SWT.NONE);
    mbtnModelPath.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        FileDialog diag = new FileDialog( parent.getShell() );
        diag.setFilterExtensions(new String[] { "*.dae;*.obj" } );
        diag.open();
        mModelPath.setText( diag.getFilterPath() + File.separator + diag.getFileName() );
      }
    });
    mbtnModelPath.setText("Browse");

    Label mLabel2 = new Label(control, SWT.NONE);
    mLabel2.setText("Select working directory");
    new Label(control, SWT.NONE);

    mOutPath = new Text(control, SWT.BORDER);
    mOutPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    Button mbtnWorkPath = new Button(control, SWT.NONE);
    mbtnWorkPath.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        DirectoryDialog diag = new DirectoryDialog( parent.getShell() );
        diag.open();
        mOutPath.setText( diag.getFilterPath() );
      }
    });
    mbtnWorkPath.setText("Browse");

    mConsoleText = new Text(control, SWT.BORDER | SWT.WRAP | SWT.V_SCROLL | SWT.MULTI);
    mConsoleText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 2, 1));
    new Label(control, SWT.NONE);
    new Label(control, SWT.NONE);
    return control;
  }

  @Override
  protected void okPressed() {
    // can we read the source file?
    String outPath = mOutPath.getText();
    String fileName = mModelPath.getText().substring( mModelPath.getText().lastIndexOf( File.separator ) + 1 );
    File in = new File( mModelPath.getText() );
    if( !in.exists() || !in.canRead() )
    {
      throw new RuntimeException("Cannot read input: " + in.getAbsolutePath() + "!");
    }

    // if exportPath was not specified, try to get the import path, minus the .dae extension.
    if(outPath.isEmpty())
    {
      outPath = mModelPath.getText().substring(0, mModelPath.getText().lastIndexOf( File.separator ) );
    }
    mOutPath.setText(outPath);
    outPath += File.separator + fileName.substring(0, fileName.lastIndexOf( "." ));
    // if the target path exists, is it writable?
    File out = new File(outPath);
    if(out.exists())
    {
      if(out.isFile())
      {
        out = out.getParentFile();
      }

      if(out == null || !out.canWrite())
      {
        throw new RuntimeException("Cannot write to folder: " + out.getAbsolutePath() + "!");
      }
    }

    int result = NativeExporter.Export(in.getAbsolutePath(), out.getAbsolutePath());
    if(result != 0)
    {
      throw new RuntimeException(NativeExporter.GetErrorMessage());
    }
    String dliContent = "";
    try {
      dliContent = new String(Files.readAllBytes(Paths.get(outPath+".dli")));

      MessageBox mb = new MessageBox(getShell());
      mb.setText("Conversion successful");
      mb.setMessage("File saved to " + outPath + " and .bin.");
      mb.open();

      mConsoleText.setText(dliContent.toString());
      GlobalData.Get().getConsolePart().AppendText(dliContent.toString());

    } catch (IOException e1) {
      e1.printStackTrace();
    }
    
    
    try {
      Project.create();
      Project.sInstance.JsonLoad(dliContent);

      GlobalData.Get().getNodeTreePart().setData();

    } catch (JsonParseException e) {
      ////////////////////////////////////////////
      //TODO: create a console with color text
      ///////////////////////////////////////////////
      
      GlobalData.Get().getConsolePart().AppendText(e.getMessage());
      //th.appendText(e.getMessage(), SWT.COLOR_RED);
    } catch (JsonMappingException e) {
      GlobalData.Get().getConsolePart().AppendText(e.getMessage());
      //th.appendText(e.getMessage(), SWT.COLOR_RED);
    } catch (IOException e) {
      GlobalData.Get().getConsolePart().AppendText(e.getMessage());
      //th.appendText(e.getMessage(), SWT.COLOR_RED);
    }
    
    
  }

  @Override
  protected void createButtonsForButtonBar(Composite parent) {
    createButton( parent, IDialogConstants.OK_ID, "Import", true );
    createButton( parent, IDialogConstants.CANCEL_ID, "Close", false );
  }

  @Override
  protected void configureShell(Shell newShell) {
    super.configureShell( newShell );
    newShell.setText( "Import model" );
  }


}
