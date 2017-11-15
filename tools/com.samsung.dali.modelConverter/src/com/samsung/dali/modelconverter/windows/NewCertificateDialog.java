package com.samsung.dali.modelconverter.windows;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.custom.ViewForm;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;

public class NewCertificateDialog extends Dialog {
	private Text mtxtProfile;
	private Text mtxtPassword;
	private Text mtxtCountry;
	private Text mtxtOrganization;
	private Text mtxtUsrName;
	private Text mtxtEmail;

	public NewCertificateDialog(Shell parentShell) {
		super(parentShell);
		setShellStyle(SWT.DIALOG_TRIM | SWT.RESIZE);
	}

	@Override
	protected Control createDialogArea(Composite parent) {

		Composite area = (Composite) super.createDialogArea(parent);
		parent.getShell().setMinimumSize(460, 300);
		
		area.setLayout(new FormLayout());
		
		Label mlb1 = new Label(area, SWT.NONE);
		FormData fd_mlb1 = new FormData();
		mlb1.setLayoutData(fd_mlb1);
		mlb1.setText("Profile:");
		
		mtxtProfile = new Text(area, SWT.BORDER);
		fd_mlb1.right = new FormAttachment(mtxtProfile, -2);
		fd_mlb1.bottom = new FormAttachment(mtxtProfile, -12, SWT.BOTTOM);
		fd_mlb1.top = new FormAttachment(mtxtProfile, 0, SWT.TOP);
		FormData fd_mtxtProfile = new FormData();
		fd_mtxtProfile.right = new FormAttachment(100, -10);
		fd_mtxtProfile.left = new FormAttachment(0, 62);
		fd_mtxtProfile.bottom = new FormAttachment(0, 39);
		fd_mtxtProfile.top = new FormAttachment(0, 10);
		mtxtProfile.setLayoutData(fd_mtxtProfile);
		
		Label mlb2 = new Label(area, SWT.NONE);
		FormData fd_mlb2 = new FormData();
		fd_mlb2.top = new FormAttachment(mtxtProfile, 6);
		fd_mlb2.left = new FormAttachment(mlb1, 0, SWT.LEFT);
		mlb2.setLayoutData(fd_mlb2);
		mlb2.setText("Password:");
		
		mtxtPassword = new Text(area, SWT.BORDER);
		fd_mlb2.bottom = new FormAttachment(mtxtPassword, -12, SWT.BOTTOM);
		FormData fd_mtxtPassword = new FormData();
		fd_mtxtPassword.right = new FormAttachment(100, -10);
		fd_mtxtPassword.left = new FormAttachment(0, 87);
		fd_mtxtPassword.top = new FormAttachment(mtxtProfile, 6);
		mtxtPassword.setLayoutData(fd_mtxtPassword);
		
		Label mlb3 = new Label(area, SWT.NONE);
		FormData fd_mlb3 = new FormData();
		fd_mlb3.left = new FormAttachment(mlb1, 0, SWT.LEFT);
		mlb3.setLayoutData(fd_mlb3);
		mlb3.setText("Country:");
		
		mtxtCountry = new Text(area, SWT.BORDER);
		fd_mlb3.top = new FormAttachment(mtxtCountry, 0, SWT.TOP);
		FormData fd_mtxtCountry = new FormData();
		fd_mtxtCountry.top = new FormAttachment(mtxtPassword, 6);
		fd_mtxtCountry.right = new FormAttachment(mtxtProfile, 0, SWT.RIGHT);
		fd_mtxtCountry.left = new FormAttachment(0, 87);
		mtxtCountry.setLayoutData(fd_mtxtCountry);
		
		Label mlb4 = new Label(area, SWT.NONE);
		FormData fd_mlb4 = new FormData();
		fd_mlb4.top = new FormAttachment(mtxtCountry, 6);
		mlb4.setLayoutData(fd_mlb4);
		mlb4.setText("Organization:");
		
		mtxtOrganization = new Text(area, SWT.BORDER);
		fd_mlb4.right = new FormAttachment(mtxtOrganization, -6);
		FormData fd_mtxtOrganization = new FormData();
		fd_mtxtOrganization.right = new FormAttachment(100, -10);
		fd_mtxtOrganization.left = new FormAttachment(0, 111);
		fd_mtxtOrganization.top = new FormAttachment(mtxtCountry, 6);
		mtxtOrganization.setLayoutData(fd_mtxtOrganization);
		
		Label mlb5 = new Label(area, SWT.NONE);
		FormData fd_mlb5 = new FormData();
		fd_mlb5.left = new FormAttachment(mlb1, 0, SWT.LEFT);
		mlb5.setLayoutData(fd_mlb5);
		mlb5.setText("User name:");
		
		mtxtUsrName = new Text(area, SWT.BORDER);
		fd_mlb5.top = new FormAttachment(mtxtUsrName, 0, SWT.TOP);
		FormData fd_mtxtUsrName = new FormData();
		fd_mtxtUsrName.left = new FormAttachment(100, -353);
		fd_mtxtUsrName.top = new FormAttachment(mtxtOrganization, 6);
		fd_mtxtUsrName.right = new FormAttachment(100, -10);
		mtxtUsrName.setLayoutData(fd_mtxtUsrName);
		
		Label mlb6 = new Label(area, SWT.NONE);
		FormData fd_mlb6 = new FormData();
		fd_mlb6.top = new FormAttachment(mlb5, 24);
		fd_mlb6.left = new FormAttachment(mlb1, 0, SWT.LEFT);
		mlb6.setLayoutData(fd_mlb6);
		mlb6.setText("email:");
		
		mtxtEmail = new Text(area, SWT.BORDER);
		FormData fd_mtxtEmail = new FormData();
		fd_mtxtEmail.right = new FormAttachment(mtxtProfile, 0, SWT.RIGHT);
		fd_mtxtEmail.top = new FormAttachment(mtxtUsrName, 6);
		fd_mtxtEmail.left = new FormAttachment(mlb6, 11);
		mtxtEmail.setLayoutData(fd_mtxtEmail);
		
		return area;
	}
}
