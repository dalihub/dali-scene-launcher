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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URISyntaxException;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.controller.FileUtils;
import com.samsung.dali.modelconverter.controller.ProjectSwitchConfirmationWorkflow;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.view.dialogs.CreateProjectDialog;

public class CreateProjectHandler {

  @Execute
  public void execute(Shell shell) {

    if (ProjectSwitchConfirmationWorkflow.execute(shell)) {

      CreateProjectDialog dialog = new CreateProjectDialog(shell);

      if (dialog.open() == 0)	// OK -- not SWT.OK.
      {
        String name = dialog.getProjectName();
        String id = dialog.getProjectID();
        String path = dialog.getProjectPath();

        try {
          ensureProjectDirectoryExists(path);
          extractProjectTemplate(path);
          instantiateManifestTemplate(path, name, id);

          Project project = new Project(path, name, id);
          GlobalData.get().setProject(project);
        }
        catch (IOException e) {
          MessageDialog.openError(shell, "Project creation failed.", e.getMessage());
        }
        catch (URISyntaxException e) {
          MessageDialog.openError(shell, "Internal error.", e.getMessage());
        }
      }
    }
  }

  private void ensureProjectDirectoryExists(String path) throws IOException {
    File directory = new File(path);
    if(!directory.exists() && !directory.mkdirs()) {
      throw new IOException("Failed to create project directory.");
    }
    else {
      File[] content = directory.listFiles();
      if(content != null && content.length > 0) {
        throw new IOException("Project directory exists and isn't empty.");
      }
    }
  }

  private void extractProjectTemplate(String path) throws IOException, URISyntaxException {

    File source = FileUtils.getBundleFile(DATA_PATH + "project-template.zip");
    FileUtils.extractZip(source, new File(path));
  }

  private void instantiateManifestTemplate(String path, String name, String id) throws IOException, URISyntaxException {
    File source = FileUtils.getBundleFile(DATA_PATH + Project.TIZEN_MANIFEST + ".template");

    String manifestContents = null;
    try (FileInputStream fis = new FileInputStream(source)) {
      byte[] data = new byte[(int) source.length()];
      fis.read(data);
      manifestContents = new String (data, "UTF-8");
    }

    manifestContents = manifestContents.replace("$(AppLabel)", name).replace("$(AppId)", id);

    File manifest = new File(path + File.separator + Project.TIZEN_MANIFEST);
    try (FileWriter writer = new FileWriter(manifest)){
      writer.write(manifestContents);
    }
  }

  private static final String DATA_PATH = "data/";
}
