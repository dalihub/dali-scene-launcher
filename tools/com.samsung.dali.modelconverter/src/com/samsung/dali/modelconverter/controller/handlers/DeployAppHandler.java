package com.samsung.dali.modelconverter.controller.handlers;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.StandardCopyOption;

import org.eclipse.e4.core.di.annotations.CanExecute;
import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.controller.FileUtils;
import com.samsung.dali.modelconverter.controller.LoggingProcessRunner;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.data.Settings;
import com.samsung.dali.modelconverter.view.dialogs.DeployAppDialog;
import com.samsung.dali.modelconverter.view.parts.GlobalParts;
import com.samsung.dali.modelconverter.view.parts.OutputPart;

public class DeployAppHandler {

  @CanExecute
  boolean canExecute() {
    String profile = Settings.Get().getString(Settings.ACTIVE_PROFILE);
    Project project = GlobalData.get().getProject();
    return profile != null && !profile.isEmpty() && project != null && project.getDocument() != null;
  }

  @Execute
  public void execute(Shell shell) {
    final String activeProfile = Settings.Get().getString(Settings.ACTIVE_PROFILE);
    final GlobalData gd = GlobalData.get();
    final Project project = gd.getProject();
    final String projectBinPath = project.getBinPath();

    DeployAppDialog cpd = new DeployAppDialog(shell);
    cpd.setPath(project.getPath());
    cpd.setCurrentProfile(activeProfile);

    if (cpd.open() == IDialogConstants.OK_ID) {
      boolean performPackaging = cpd.isPerformPackagingRequested();
      boolean performInstallation = cpd.isPerformInstallationRequested();

      try {
        // Copy watch loader
        File loader = FileUtils.getBundleFile("data/" + WATCHLOADER_NAME);
        File target = new File(projectBinPath + WATCHLOADER_NAME);
        FileUtils.copyFile(loader, target, StandardCopyOption.REPLACE_EXISTING);

        // Execute packaging command
        OutputPart op = GlobalParts.getOutputPart();
        LoggingProcessRunner lpr = LoggingProcessRunner.create(shell.getDisplay(), op.getText());

        if (performPackaging) {
          lpr.addCommand(gd.getTizenPath() + " package -t tpk -s " + activeProfile + " -- " + projectBinPath);
        }

        String tpkPath = projectBinPath + project.getId() + "-" + project.getVersion() + "-arm.tpk";
        if (performInstallation) {
          lpr.addCommand(gd.getSdbPath() + " install " + tpkPath);
        }

        lpr.run(true);
      }
      catch (URISyntaxException | IOException e) {
        MessageDialog.openError(shell, "Package creation failed.", e.getMessage());
      }
    }
  }

  private static final String WATCHLOADER_NAME = "watchloader";
}
