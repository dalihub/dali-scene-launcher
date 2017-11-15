package com.samsung.dali.modelconverter.windows;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelconverter.data.SettingsClass;

import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.custom.CCombo;

public class SelectCertificateDialog extends Dialog {
	
	public SelectCertificateDialog(Shell parentShell) {
		super(parentShell);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite control = (Composite) super.createDialogArea(parent);
		parent.getShell().setMinimumSize(400, 200);
		control.setLayout(new GridLayout(4, false));
		
		Label lblNewLabel_1 = new Label(control, SWT.NONE);
		lblNewLabel_1.setText("Select certificate");
		
		Label lblNewLabel = new Label(control, SWT.NONE);
		GridData gd_lblNewLabel = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
		gd_lblNewLabel.widthHint = 50;
		lblNewLabel.setLayoutData(gd_lblNewLabel);
		new Label(control, SWT.NONE);
		new Label(control, SWT.NONE);
		
		CCombo mCmbCertificate = new CCombo(control, SWT.NONE);
		mCmbCertificate.setItems(new String[] {"One", "Two"});
		mCmbCertificate.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		new Label(control, SWT.NONE);
		new Label(control, SWT.NONE);
		
		Button mbtnNewCert = new Button(control, SWT.NONE);
		mbtnNewCert.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				NewCertificateDialog newCert = new NewCertificateDialog(parent.getShell());
				newCert.open();
				
			}
		});
		mbtnNewCert.setText("Create certificate");

		return control;
	}

	@Override
	protected void okPressed() {
		// TODO Auto-generated method stub
		super.okPressed();
	}
	
	
}
