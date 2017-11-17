package com.samsung.dali.modelConverter.handlers;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.SWT;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.samsung.dali.modelConverter.model.Project;
import com.samsung.dali.modelConverter.parts.OutputPart;
import com.samsung.dali.modelConverter.parts.SceneGraphPart;
import com.samsung.dali.modelConverter.view.TextAppendHelper;

public class ProjectScopeHandlerBase {

  protected boolean canLeaveProjectScope()
  {
    boolean allow = !Project.isOpen();
    if(!allow)
    {
      MessageDialog dialog = new MessageDialog(
          null, "Title", null, "This will close the current project. Do you want to proceed?",
          MessageDialog.QUESTION,
          new String[] {"Cancel", "Proceed" },
          0); // Cancel is the default
       int result = dialog.open();
       
       allow = result == 1;
    }
    return allow;
  }

  protected void completeLoading(String inputPath, String dli)
  {
    if(dli != null) {
      try {
        TextAppendHelper th = new TextAppendHelper(OutputPart.sActiveInstance.getOutputPanel());
        
        try {
          Project.create();
          Project.sInstance.deserialize(dli);

          Project.sInstance.binLocation = inputPath.substring(0, inputPath.length() - ".dae".length());
          Project.sInstance.dliLocation = Project.sInstance.binLocation + ".dli";
          Project.sInstance.binLocation += ".bin";
          
          mMostRecent = inputPath;
          
          th.appendText(dli);
          
          SceneGraphPart sceneGraph = SceneGraphPart.sActiveInstance;
          sceneGraph.populate(Project.sInstance.getDocument());

        } catch (JsonParseException e) {
          th.appendText(e.getMessage(), SWT.COLOR_RED);
        } catch (JsonMappingException e) {
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

  protected String mMostRecent = "";
}
