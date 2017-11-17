package com.samsung.dali.modelConverter.handlers;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Shell;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.samsung.dali.modelConverter.model.Project;
import com.samsung.dali.modelConverter.parts.OutputPart;
import com.samsung.dali.modelConverter.view.TextAppendHelper;

public class SaveHandler {

  @Execute
  public void execute(IWorkbench workbench, Shell shell) {
    
    if(Project.isOpen())  // TODO: disable command if not even.
    {
      TextAppendHelper th = new TextAppendHelper(OutputPart.sActiveInstance.getOutputPanel());
      try {
        try {
          String json = Project.sInstance.serialize();
          
          File file = new File(Project.sInstance.dliLocation);
          try(FileWriter writer = new FileWriter(file)) {
            writer.write(json);
          }
        } catch (JsonProcessingException e) {
          th.appendText(e.getMessage(), SWT.COLOR_RED);
        } catch (IOException e) {
          th.appendText(e.getMessage(), SWT.COLOR_RED);
        }
      } catch (UnsupportedEncodingException e1) {
        // TODO Auto-generated catch block
        e1.printStackTrace();
      } catch (IOException e1) {
        // TODO Auto-generated catch block
        e1.printStackTrace();
      }
    }
  }
}
