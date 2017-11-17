package com.samsung.dali.modelConverter.handlers;

import java.io.File;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.model.Project;
import com.samsung.dali.modelConverter.parts.OutputPart;
import com.samsung.dali.modelConverter.process.LoggingProcessRunner;

public class CreateProjectHandler extends ProjectScopeHandlerBase {

  @Execute
  public void execute(IWorkbench workbench, Shell shell) {
    
    if(canLeaveProjectScope())
    {
      Project.close();
      
      DirectoryDialog dd = new DirectoryDialog(shell);
      dd.setFilterPath(mMostRecent);

      String projectPath = dd.open();
      if(projectPath != null) {
        
        boolean isWindows = System.getProperty("os.name").toLowerCase().contains("windows");
        
        String tizenCmd = "tizen";
        if(isWindows)
        {
          tizenCmd += ".bat";
        }
        
        String projectName = new File(projectPath).getName(); // project name is the name of the folder.
        //projectPath = new File(projectPath).getParent();  // TODO: the tizen tool uses the name of the project to create a subdirectory here, resulting in 'path/to/my/project/project'. We cannot move up a level, as it'll cause 'folder already exists' error. We cannot delete the folder as it might have contents. We'll need to present it differently - TBD.
        String profile = "wearable-3.0"; // TODO: an option would be great.
        
        LoggingProcessRunner.create(shell.getDisplay(),
          OutputPart.sActiveInstance.getOutputPanel())
          .addCommand(tizenCmd + " create native-project -p " + profile + " -t watchapp -n " +
            projectName + " -- " + projectPath)
          .run();
      }
    }
  }
  
}
