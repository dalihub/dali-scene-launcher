package com.samsung.dali.modelConverter.handlers;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.windows.ImportDialog;

public class ImportHandler {
	@Execute
	public void execute(Shell shell) {
		FileDialog dialog = new FileDialog(shell);
		dialog.open();
		//ImportDialog is not necessary now, the model is loaded to string, not to file
		//ImportDialog dialog = new ImportDialog(shell);
		//dialog.open();
	}
	
	
	
	
}
