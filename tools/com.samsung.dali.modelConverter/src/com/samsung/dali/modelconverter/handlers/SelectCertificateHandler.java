package com.samsung.dali.modelconverter.handlers;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.windows.SelectCertificateDialog;

public class SelectCertificateHandler {
	@Execute
	public void execute(Shell shell) {
		SelectCertificateDialog dialog = new SelectCertificateDialog(shell);
		dialog.open();
	}
}
