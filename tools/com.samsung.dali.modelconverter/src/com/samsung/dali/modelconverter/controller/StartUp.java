package com.samsung.dali.modelconverter.controller;

import java.io.File;

import org.eclipse.e4.ui.workbench.lifecycle.PostContextCreate;
import org.eclipse.equinox.app.IApplicationContext;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.view.dialogs.TizenPathDialog;

@SuppressWarnings("restriction")
public class StartUp {

  @PostContextCreate
  void postContextCreate(Display display, IApplicationContext context) {
    GlobalData.initialize();

    verifyTizenPath(display.getActiveShell(), false);
  }

  private void verifyTizenPath(Shell shell, boolean force) {

    // Check if the path we know is still a-ok.
    if(!checkToolsError(shell)) {
      force = true; // we must request them again.
    }

    GlobalData globalData = GlobalData.get();
    String cliPath = globalData.getRootTizenPath();
    if(force) {
      cliPath = ""; // discard what we have.
    }

    while(cliPath.isEmpty()) {
      TizenPathDialog pathTizen = new TizenPathDialog(shell);

      int result = pathTizen.open();
      switch(result) {
      case 0: // OK
        cliPath = pathTizen.getResult();
        globalData.setRootTizenPath(cliPath);

        if(!checkToolsError(shell)) {
          cliPath = ""; // discard what we have as it doesn't work.
        }
        break;

      default:
        System.exit(0);
        // TODO: This is not the recommended way to shut down, however we don't have a Workbench
        // at this point, for workbench.close(). The context.get(IWorkbench.class) recommendation
        // from https://stackoverflow.com/questions/28507982/eclipse-rcp-how-to-shutdown-before-workbench-initializes#28508748
        // does not work.
      }
    }

    globalData.setRootTizenPath(cliPath);
  }

  private boolean checkToolsError(Shell shell)
  {
    String error = validateTools(shell);
    boolean isValid = error == null;
    if(!isValid)
    {
      MessageDialog.openError(shell, "Invalid path", error);
    }
    return isValid;
  }

  private String validateTools(Shell shell)
  {
    GlobalData globalData = GlobalData.get();
    File sdb = new File(globalData.getSdbPath());
    File tizen = new File(globalData.getTizenPath());
    String error = null;
    if(!sdb.exists()) {
      error = "Failed to find " + globalData.getSdbPath() + ".";
    }

    if(!tizen.exists()) {
      error = "Failed to find " + globalData.getTizenPath() + ".";
    }

    if(!sdb.canExecute()) {
      error = "Lacking execute permission on " + globalData.getSdbPath() + ".";
    }

    if(!tizen.exists()) {
      error = "Lacking execute permission on " + globalData.getTizenPath() + ".";
    }

    return error;
  }
}
