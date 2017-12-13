package com.samsung.dali.modelconverter.controller.handlers;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.eclipse.e4.core.di.annotations.CanExecute;
import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.controller.SceneUpdateWorkflow;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelexporter.ModelExporter;

public class ImportSceneHandler {

  enum MergeBehaviour {
    Overwrite,
    // ... TBD: Additional behaviours, e.g. Add, Update.
    Cancel,
  }

  static String[] getImportBehaviourStrings() {
    MergeBehaviour[] values = MergeBehaviour.values();
    String[] strings = new String[values.length];
    for (int i = 0; i < strings.length; ++i) {
      strings[i] = values[i].toString();
    }
    return strings;
  }

  @CanExecute
  boolean canExecute() {
    return GlobalData.get().getProject() != null;
  }

  @Execute
  void execute(Shell shell, EPartService parts) {
    FileDialog dialog = new FileDialog(shell);
    dialog.setFilterExtensions(new String[]
      { "*.dli;*.dae" });

    String inputPath = dialog.open();
    if (inputPath != null && !inputPath.isEmpty()) {
      GlobalData globalData = GlobalData.get();
      Project project = globalData.getProject();
      assert project != null; // The option must be disabled when we have no project open.

      MergeBehaviour behaviour = MergeBehaviour.Overwrite;
      if (new File(project.getSceneDliPath()).exists()) {
        MessageDialog md = new MessageDialog(shell, "Select Import Behaviour", null,
            "A scene already exists in the project. What would you like to do?\n"
                + "'Overwrite' replaces the old scene with the new one.\n"
                + "'Cancel' aborts the operation and leaves the old scene intact.",
            MessageDialog.QUESTION, getImportBehaviourStrings(), MergeBehaviour.Cancel.ordinal());

        behaviour = MergeBehaviour.values()[md.open()];
      }

      if (behaviour != MergeBehaviour.Cancel) {

        // Get .bin name. Must be the same as the name of the .dli.
        int iPeriod = inputPath.lastIndexOf('.');
        assert iPeriod != -1; // Since we supposed to have filtered.
        String binName = inputPath.substring(0, iPeriod + 1) + "bin";

        String dli = null;
        byte[] bin = null;
        try {
          if (inputPath.endsWith(".dae")) {
            // Can we read the source file?
            File inFile = new File(inputPath);
            if (!inFile.exists() || !inFile.canRead()) {
              throw new IOException("Cannot read input: " + inFile.getAbsolutePath() + "!");
            }

            // Is the output directory exists and writable?
            File out = new File(project.getModelsPath());
            if (!(out.exists() && out.isDirectory() && out.canWrite())) {
              throw new IOException("Cannot write to folder: " + out.getAbsolutePath() + "!");
            }

            if (ModelExporter.nativeConvert(inputPath, out.getAbsolutePath()) == 0) {
              dli = ModelExporter.nativeGetDliContents();
              bin = ModelExporter.nativeGetBinContents();
            }
            else {
              throw new IOException("Conversion failed: " + ModelExporter.nativeGetErrorMessage());
            }
          }
          else {
            dli = new String(Files.readAllBytes(Paths.get(inputPath)));
            bin = Files.readAllBytes(Paths.get(binName));
          }

          Document newDoc = Document.fromDli(dli);

          switch (behaviour) {
          case Overwrite:
            project.setDocument(newDoc);
            break;
          default:
            throw new UnsupportedOperationException(behaviour.toString());
          }

          newDoc.setNodeParents();
          project.setDocument(newDoc);

          SceneUpdateWorkflow.execute(shell, parts);
        }
        catch (IOException e) {
          MessageDialog.openError(shell, "Failed to import scene.", e.getMessage());
        }

        // Write .dli and .bin files to project directory.
        try {
          Files.write(Paths.get(project.getSceneDliPath()), dli.getBytes());
          Files.write(Paths.get(project.getSceneBinPath()), bin);
        }
        catch (IOException e) {
          MessageDialog.openError(shell, "Failed to import scene.", e.getMessage());
        }
      }
    }
  }
}
