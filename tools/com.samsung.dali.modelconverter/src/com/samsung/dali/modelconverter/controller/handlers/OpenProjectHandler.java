package com.samsung.dali.modelconverter.controller.handlers;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import javax.xml.parsers.ParserConfigurationException;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;
import org.xml.sax.SAXException;

import com.samsung.dali.modelconverter.controller.ProjectSwitchConfirmationWorkflow;
import com.samsung.dali.modelconverter.controller.SceneUpdateWorkflow;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.data.document.Document;

public class OpenProjectHandler {

  @Execute
  public void execute(Shell shell, EPartService parts) {

    if (ProjectSwitchConfirmationWorkflow.execute(shell)) {
      DirectoryDialog dialog = new DirectoryDialog(shell);
      String path = dialog.open();

      if (path != null && !path.isEmpty()) {
        GlobalData globalData = GlobalData.get();

        try {
          Project project = Project.open(path);
          globalData.setProject(project);

        }
        catch (ParserConfigurationException | SAXException e) {

          MessageDialog.openError(shell, "Error opening project.", "Failed to parse manifest: " + e.getMessage());
          return;
        }
        catch (IOException e) {

          MessageDialog.openError(shell, "Error opening project.", "I/O error: " + e.getMessage());
          return;
        }

        Project project = globalData.getProject();
        try {
          String dli = new String(Files.readAllBytes(Paths.get(project.getSceneDliPath())));
          project.setDocument(Document.fromDli(dli));

          SceneUpdateWorkflow.execute(shell, parts);
        }
        catch (IOException e) {
          MessageDialog.openWarning(shell, "Failed to open project scene.", e.getMessage());
        }
      }
    }
  }
}
