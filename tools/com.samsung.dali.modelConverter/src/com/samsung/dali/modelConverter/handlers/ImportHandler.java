package com.samsung.dali.modelConverter.handlers;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.model.ModelConverterLogic;
import com.samsung.dali.modelConverter.model.Project;

public class ImportHandler extends ProjectScopeHandlerBase {

  @Execute
  public void execute(IWorkbench workbench, Shell shell) {
    
    if(canLeaveProjectScope())
    {
      FileDialog fd = new FileDialog(shell);
      fd.setFilterExtensions(new String[] { "*.dae" });
      fd.setFilterPath(mMostRecent);
      
      String inputPath = fd.open();
      if(!inputPath.isEmpty())
      {
        Project.close();
        
        // perform conversion
        ModelConverterLogic model = ModelConverterLogic.getInstance();
        model.setImportPath(inputPath);
        // TODO: move the .bin file from this location. Ideally create it in some temporary location and move from there.
  
        String dli = model.performConversion();
        completeLoading(inputPath, dli);
      }
    }    
  }
}
