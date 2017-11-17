package com.samsung.dali.modelConverter.handlers;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.model.Project;
import com.samsung.dali.modelConverter.parts.OutputPart;
import com.samsung.dali.modelConverter.view.TextAppendHelper;

public class OpenHandler extends ProjectScopeHandlerBase {

  @Execute
  public void execute(IWorkbench workbench, Shell shell) {
    
    if(canLeaveProjectScope())
    {
      FileDialog fd = new FileDialog(shell);
      fd.setFilterExtensions(new String[] { "*.dli" });
      fd.setFilterPath(mMostRecent);
      
      String inputPath = fd.open();
      if(!inputPath.isEmpty())
      {
        Project.close();
        
        String dli = null;
        try {
          dli = new String(Files.readAllBytes(Paths.get(inputPath)), StandardCharsets.UTF_8);
        } catch (IOException e) {

          try {
            new TextAppendHelper(OutputPart.sActiveInstance.getOutputPanel()).appendText(e.getMessage());
          } catch (UnsupportedEncodingException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
          } catch (IOException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
          };
        }

        completeLoading(inputPath, dli);
      }
    }    
  }
}
