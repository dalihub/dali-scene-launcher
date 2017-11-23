package com.samsung.dali.modelconverter.controller;

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

import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.view.dialogs.TizenPathDialog;

public class TizenPathDialogController {

  public TizenPathDialogController(TizenPathDialog tizenPathDialog) {
    this.view = tizenPathDialog;
  }

  static public void verifyTizenPath(Shell shell, boolean force) {
    String cliPath = GlobalData.get().getRootTizenPath();
    if(cliPath.isEmpty() || force)
    {
      TizenPathDialog pathTizen = new TizenPathDialog(shell);
      pathTizen.open();
    }
  }

  public void onBrowse(){
    String path;
    DirectoryDialog dialog = new DirectoryDialog( view.getShell() );
    dialog.open();
    path = dialog.getFilterPath();
    view.mPath.setText(path);
  }

  public void okPressed(){
    String cliPath = view.mPath.getText() + File.separator;
    GlobalData.get().setRootTizenPath(cliPath);
  }

  TizenPathDialog view;
}
