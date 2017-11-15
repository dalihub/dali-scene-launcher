package com.samsung.dali.modelConverter.handlers;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.SettingsClass;
import com.samsung.dali.modelConverter.windows.InstallOnDeviceDialog;

public class InstallOnDeviceHandler {

  @Execute
  public void execute(Shell shell) {
    String path = GlobalData.Get().mInstallPackagePath;
    if(path.isEmpty()) {
      DirectoryDialog dialog = new DirectoryDialog(shell);
      dialog.setMessage("Select Package Directory");
      dialog.open();
      GlobalData.Get().mInstallPackagePath = dialog.getFilterPath();
      SettingsClass.Get().SetString( SettingsClass.TIZEN_CURRENT_WORKPATH, GlobalData.Get().mInstallPackagePath );
    }
    InstallOnDeviceDialog diag = new InstallOnDeviceDialog( shell );
    diag.open();

  }
}
