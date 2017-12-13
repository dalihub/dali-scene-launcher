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

import java.util.ArrayList;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.controller.ListProfilesWorkflow;
import com.samsung.dali.modelconverter.data.Settings;
import com.samsung.dali.modelconverter.view.dialogs.ManageProfilesDialog;

public class ManageProfilesHandler {

  @Execute
  void execute(Shell shell, EPartService parts) {

    // Get list of security profiles.
    ArrayList<String> securityProfiles = new ArrayList<String>();
    ListProfilesWorkflow.execute(shell, securityProfiles, parts);

    // Create and populate dialog.
    ManageProfilesDialog manageDialog = new ManageProfilesDialog(shell);
    manageDialog.setProfiles(securityProfiles);

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
}
