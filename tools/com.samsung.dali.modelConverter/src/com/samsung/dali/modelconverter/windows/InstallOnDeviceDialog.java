package com.samsung.dali.modelconverter.windows;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

public class InstallOnDeviceDialog extends Dialog {
	static String mPackageDirectory = "";
	private Text mPath;
	private Text mConsole;

	/**
	 * @wbp.parser.constructor
	 */
	public InstallOnDeviceDialog(Shell parentShell, String path) {
		super(parentShell);
		mPackageDirectory = path;
	}
	static public String GetWorkingPath() {
		return mPackageDirectory;
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
		
		Button mbtnBrowse = new Button(area, SWT.NONE);
		mbtnBrowse.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
			}
		});
		fd_mPath.right = new FormAttachment(mbtnBrowse, -6);
		FormData fd_mbtnBrowse = new FormData();
		fd_mbtnBrowse.top = new FormAttachment(0, 33);
		fd_mbtnBrowse.right = new FormAttachment(100, -10);
		mbtnBrowse.setLayoutData(fd_mbtnBrowse);
		mbtnBrowse.setText("Browse");
		
		Label mlb1 = new Label(area, SWT.NONE);
		FormData fd_mlb1 = new FormData();
		fd_mlb1.top = new FormAttachment(0, 10);
		fd_mlb1.left = new FormAttachment(0, 10);
		mlb1.setLayoutData(fd_mlb1);
		mlb1.setText("Working directory");
		
		Button mChkPackage = new Button(area, SWT.CHECK);
		mChkPackage.setSelection(true);
		FormData fd_mChkPackage = new FormData();
		fd_mChkPackage.top = new FormAttachment(mPath, 6);
		fd_mChkPackage.left = new FormAttachment(mPath, 0, SWT.LEFT);
		mChkPackage.setLayoutData(fd_mChkPackage);
		mChkPackage.setText("Create package");
		
		Button mChkInstall = new Button(area, SWT.CHECK);
		mChkInstall.setSelection(true);
		FormData fd_mChkInstall = new FormData();
		fd_mChkInstall.top = new FormAttachment(mChkPackage, 6);
		fd_mChkInstall.left = new FormAttachment(mPath, 0, SWT.LEFT);
		mChkInstall.setLayoutData(fd_mChkInstall);
		mChkInstall.setText("Install on device");
		
		mPath.setText( mPackageDirectory );
		
		mConsole = new Text(area, SWT.BORDER | SWT.MULTI);
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
		mCurrentProfile.setText("Profile");
		return area;
	}

	@Override
	protected void okPressed() {
		// TODO Auto-generated method stub
		super.okPressed();
	}
	
	
}
