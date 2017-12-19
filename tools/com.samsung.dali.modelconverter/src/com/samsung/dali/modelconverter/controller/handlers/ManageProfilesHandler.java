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
package com.samsung.dali.modelconverter.controller.handlers;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.controller.ListProfilesWorkflow;
import com.samsung.dali.modelconverter.controller.LoggingProcessRunner;
import com.samsung.dali.modelconverter.controller.PartsHelper;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Settings;
import com.samsung.dali.modelconverter.view.dialogs.CreateProfileDialog;
import com.samsung.dali.modelconverter.view.dialogs.ManageProfilesDialog;
import com.samsung.dali.modelconverter.view.parts.OutputPart;

public class ManageProfilesHandler {

  @Execute
  void execute(Shell shell, EPartService parts) {

    // Get list of security profiles.
    ArrayList<String> securityProfiles = new ArrayList<String>();
    ListProfilesWorkflow.execute(shell, securityProfiles, parts);

    // Create and populate dialog.
    ManageProfilesDialog manageDialog = new ManageProfilesDialog(shell);
    manageDialog.setProfiles(securityProfiles);
    manageDialog.setCreateProfileListener(new SelectionListener() {

      @Override
      public void widgetSelected(SelectionEvent e) {

        handleCreateProfile(parts, manageDialog, securityProfiles);
      }

      @Override
      public void widgetDefaultSelected(SelectionEvent e) {
        // TODO Auto-generated method stub

      }

    });

    String activeProfile = Settings.Get().getString(Settings.ACTIVE_PROFILE);
    int selection = securityProfiles.indexOf(activeProfile);
    if (selection == -1) {
      selection = 0;
    }
    manageDialog.selectProfile(selection);

    // Get user input.
    if (manageDialog.open() == IDialogConstants.OK_ID) {
      Settings.Get().setString(Settings.ACTIVE_PROFILE, manageDialog.getResult());
    }
  }

  protected void handleCreateProfile(EPartService parts, ManageProfilesDialog manageDialog,
      List<String> securityProfiles) {
    Shell shell = manageDialog.getShell();
    CreateProfileDialog createDialog = new CreateProfileDialog(shell);
    createDialog.SetExistingProfiles(securityProfiles);
    if (createDialog.open() == IDialogConstants.OK_ID) {
      OutputPart op = PartsHelper.getPart(parts, OutputPart.class);

      // Create dialog if it was requested.
      GlobalData gd = GlobalData.get();

      File certFile = new File(createDialog.getCertificatePath());

      // Inline implementation of LoggingProcessRunner.Parser can only operate
      // on a final object, but it's purpose is to set a string, therefore --
      final class StringHolder {
        public String value;
      }
      final StringHolder pathHolder = new StringHolder();
      pathHolder.value = certFile.getParent();

      LoggingProcessRunner lpr = LoggingProcessRunner.create(shell.getDisplay(), op.getText());
      if (createDialog.isCreatingCertificate()) {
        // When creating the certificate, we just want to supply a name - tizen will add
        // .p12; don't duplicate it.
        String certName = certFile.getName();
        if (certName.endsWith(".p12")) {
          certName = certName.substring(0, certName.length() - ".p12".length());
        }

        String command = gd.getTizenPath() + " certificate -a " + quoted(createDialog.getAlias()) + " -p "
            + quoted(createDialog.getPassword()) + " -f " + quoted(certName);

        if (!createDialog.getCountry().isEmpty()) {
          command += " -c " + createDialog.getCountry();
        }

        if (!createDialog.getCity().isEmpty()) {
          command += " -ct " + quoted(createDialog.getCity());
        }

        if (!createDialog.getOrganization().isEmpty()) {
          command += " -o " + quoted(createDialog.getOrganization());
        }

        if (!createDialog.getEmail().isEmpty()) {
          command += " -e " + createDialog.getEmail();
        }

        if (pathHolder.value != null && !pathHolder.value.isEmpty()) {
          command += " -- " + pathHolder.value;
        }

        lpr.addCommand(command, new LoggingProcessRunner.Parser() {

          private static final String MESSAGE_START = "No path option. Will be generated on '";
          private static final String MESSAGE_END = "'.";

          @Override
          public void parseLine(String line) {
            if (line.startsWith(MESSAGE_START)) {
              pathHolder.value = line.substring(MESSAGE_START.length(), line.length() - MESSAGE_END.length());
            }
          }

        });

        // Since we need the output from the first command, we'll need to run it
        // separately.
        lpr.run(false);
      }

      if (lpr.getResult() == 0) {
        // Now create the profile. Prepend with path, and if the extension is missing,
        // add it here, since now we're referring to a file.
        if (pathHolder.value != null) {
          if (!pathHolder.value.endsWith(File.separator)) {
            pathHolder.value += File.separator;
          }
        }
        else {
          pathHolder.value = "";
        }
        pathHolder.value += certFile.getName();

        if (!pathHolder.value.endsWith(".p12")) {
          pathHolder.value += ".p12";
        }

        lpr = LoggingProcessRunner.create(shell.getDisplay(), op.getText());
        String command = gd.getTizenPath() + " security-profiles add -n " + quoted(createDialog.getAlias()) + " -a "
            + quoted(pathHolder.value) + " -p " + quoted(createDialog.getPassword());

        lpr.addCommand(command);

        lpr.run(false);
        if (lpr.getResult() == 0) {
          securityProfiles.add(createDialog.getAlias());
          manageDialog.setProfiles(securityProfiles);
          manageDialog.selectProfile(securityProfiles.size() - 1, true);
        }
      }
    }
  }

  private static String quoted(String value) {
    if (value.isEmpty() || value.contains(" ")) {
      value = "\"" + value + "\"";
    }
    return value;
  }
}
