package com.samsung.dali.modelConverter.windows;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.window.IShellProvider;
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

import com.samsung.dali.modelConverter.process.ILogText;
import com.samsung.dali.modelConverter.process.NativeExporter;

import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.ExpandBar;
import org.eclipse.swt.widgets.ExpandItem;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.layout.FillLayout;

public class ImportDialog extends Dialog {
  private Control control;
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
    
    try {
      String dliContent = new String(Files.readAllBytes(Paths.get(outPath+".dli")));

      MessageBox mb = new MessageBox(getShell());
      mb.setText("Conversion successful");
      mb.setMessage("File saved to " + outPath + " and .bin.");
      mb.open();

      mConsoleText.setText(dliContent.toString());

    } catch (IOException e1) {
      e1.printStackTrace();
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
