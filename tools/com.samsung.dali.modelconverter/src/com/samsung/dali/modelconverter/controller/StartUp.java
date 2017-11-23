package com.samsung.dali.modelconverter.controller;

import org.eclipse.e4.ui.workbench.lifecycle.PostContextCreate;
import org.eclipse.equinox.app.IApplicationContext;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.view.dialogs.TizenPathDialog;

public class StartUp {

  @PostContextCreate
  void postContextCreate(Display display, IApplicationContext context) {
    GlobalData.initialize();
    
    verifyTizenPath(display.getActiveShell(), true);
  }

  private void verifyTizenPath(Shell shell, boolean force) {

    String cliPath = "";
    if(!force)
    {
      cliPath = GlobalData.get().getRootTizenPath();
    }
    
    while(cliPath.isEmpty())
    {
      TizenPathDialog pathTizen = new TizenPathDialog(shell);
      int result = pathTizen.open(); 
      switch(result)
      {
      case 0: // OK
        cliPath = pathTizen.getResult();
        break;
      
      default:
        System.exit(0);
        // TODO: This is not the recommended way to shut down, however we don't have a Workbench
        // at this point, for workbench.close(). The context.get(IWorkbench.class) recommendation
        // from https://stackoverflow.com/questions/28507982/eclipse-rcp-how-to-shutdown-before-workbench-initializes#28508748
        // does not work.
      }
    }
    
    GlobalData.get().setRootTizenPath(cliPath);
  }
}
