package com.samsung.dali.modelConverter.windows;

import java.io.File;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelConverter.data.SettingsClass;

import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

public class TizenPathDialog extends Dialog {
	private Text mPath;

	public TizenPathDialog(Shell parentShell) {
		super(parentShell);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite area = (Composite) super.createDialogArea(parent);
		area.setLayout(new FormLayout());
		
		Label mlb1 = new Label(area, SWT.NONE);
		FormData fd_mlb1 = new FormData();
		fd_mlb1.top = new FormAttachment(0, 10);
		fd_mlb1.left = new FormAttachment(0, 10);
		mlb1.setLayoutData(fd_mlb1);
		mlb1.setText("Tizen tools not found");
		
		Label mlb2 = new Label(area, SWT.NONE);
		FormData fd_mlb2 = new FormData();
		fd_mlb2.top = new FormAttachment(mlb1, 6);
		fd_mlb2.left = new FormAttachment(mlb1, 0, SWT.LEFT);
		mlb2.setLayoutData(fd_mlb2);
		mlb2.setText("Select Tizen CLI tools path (tizen-studio/ directory)");
		
		mPath = new Text(area, SWT.BORDER);
		FormData fd_mPath = new FormData();
		fd_mPath.top = new FormAttachment(0, 56);
		fd_mPath.left = new FormAttachment(0, 10);
		mPath.setLayoutData(fd_mPath);
		
		Button mbtnBrowse = new Button(area, SWT.NONE);
		fd_mPath.right = new FormAttachment(mbtnBrowse, -6);
		mbtnBrowse.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				String path;
				DirectoryDialog dialog = new DirectoryDialog( parent.getShell() );
				dialog.open();
				path = dialog.getFilterPath();
				mPath.setText(path);
			}
		});
		FormData fd_mbtnBrowse = new FormData();
		fd_mbtnBrowse.top = new FormAttachment(0, 56);
		fd_mbtnBrowse.right = new FormAttachment(100, -10);
		mbtnBrowse.setLayoutData(fd_mbtnBrowse);
		mbtnBrowse.setText("Browse");
		return area;
	}

	@Override
	protected void okPressed() {
		String cliPath = mPath.getText() + File.separator;
		SettingsClass.Get().SetString(SettingsClass.TIZEN_CLI_PATH, cliPath);
		super.okPressed();
	}
	
}
