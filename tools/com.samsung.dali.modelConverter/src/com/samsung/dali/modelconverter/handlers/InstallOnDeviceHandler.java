package com.samsung.dali.modelconverter.handlers;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.windows.InstallOnDeviceDialog;

public class InstallOnDeviceHandler {
	
	@Execute
	public void execute(Shell shell) {
		//MessageDialog.openInformation(shell, "Install", "InstallOnDeviceHandler");
		String path = InstallOnDeviceDialog.GetWorkingPath();
		if(path.isEmpty()) {
			DirectoryDialog dialog = new DirectoryDialog(shell);
			dialog.open();
			path = dialog.getText();
		}
		InstallOnDeviceDialog diag = new InstallOnDeviceDialog( shell, path );
		diag.open();
		
	}
}
