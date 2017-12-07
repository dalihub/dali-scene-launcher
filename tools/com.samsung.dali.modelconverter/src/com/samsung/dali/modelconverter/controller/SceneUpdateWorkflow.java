package com.samsung.dali.modelconverter.controller;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.view.parts.GlobalParts;

/*
 * Gets the document from the current project and updates the SceneGraph & Dli views.
 */
public class SceneUpdateWorkflow {

  public static void execute(Shell shell) {

    GlobalData globalData = GlobalData.get();
    Project project = globalData.getProject();
    Document doc = project.getDocument();

    SceneGraphContentProvider provider = new SceneGraphContentProvider(doc);
    GlobalParts.getSceneGraphPart().populate(provider);

    String dli = "<ERROR>";
    try {
      dli = doc.toDliString();
    }
    catch (JsonProcessingException e) {
      MessageDialog.openError(shell, "Error opening scene.", e.getMessage());
    }

    GlobalParts.getDliTextPart().populate(dli);
  }
}
