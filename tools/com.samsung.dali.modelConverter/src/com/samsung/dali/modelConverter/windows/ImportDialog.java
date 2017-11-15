package com.samsung.dali.modelConverter.windows;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.ExpandBar;
import org.eclipse.swt.widgets.ExpandItem;
import org.eclipse.swt.layout.FillLayout;

public class ImportDialog extends Dialog {
	private Control control;
	private Text mModelPath;
	private Text mWorkPath;
	private Text mConsoleText;

	public ImportDialog(Shell parentShell) {
		super(parentShell);
		setShellStyle(SWT.DIALOG_TRIM | SWT.RESIZE);
		
	}

	@Override
	protected Control createDialogArea(Composite parent) {

		Composite control = (Composite) super.createDialogArea(parent);
		parent.getShell().setMinimumSize(400, 200);
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
			}
		});
		mbtnModelPath.setText("Browse");
		
		Label mLabel2 = new Label(control, SWT.NONE);
		mLabel2.setText("Select working directory");
		new Label(control, SWT.NONE);
		
		mWorkPath = new Text(control, SWT.BORDER);
		mWorkPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		Button mbtnWorkPath = new Button(control, SWT.NONE);
		mbtnWorkPath.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
			}
		});
		mbtnWorkPath.setText("Browse");
		
		mConsoleText = new Text(control, SWT.BORDER | SWT.WRAP | SWT.MULTI);
		mConsoleText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 2, 1));
		new Label(control, SWT.NONE);
		new Label(control, SWT.NONE);
		return control;
	}
	
    @Override
    protected void okPressed() {
    	//TODO: Import
    }
    
	@Override
	protected void createButtonsForButtonBar(Composite parent) {
        createButton(parent, IDialogConstants.OK_ID, "Import", true);
        createButton(parent, IDialogConstants.CANCEL_ID,
        		"Close", false);
	}

	
}
