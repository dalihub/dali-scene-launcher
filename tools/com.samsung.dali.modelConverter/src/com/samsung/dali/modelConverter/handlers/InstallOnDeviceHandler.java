package com.samsung.dali.modelConverter.handlers;

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

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.SettingsClass;
import com.samsung.dali.modelConverter.windows.InstallOnDeviceDialog;

public class InstallOnDeviceHandler {

  @Execute
  public void execute(Shell shell) {
    String path = GlobalData.Get().GetInstallPackagePath();
    if( path.isEmpty() ) {
      DirectoryDialog dialog = new DirectoryDialog( shell );
      dialog.setMessage( "Select Package Directory" );
      dialog.open();
      GlobalData.Get().SetInstallPackagePath( dialog.getFilterPath() );
      SettingsClass.Get().SetString( SettingsClass.TIZEN_CURRENT_WORKPATH, GlobalData.Get().GetInstallPackagePath() );
    }
    InstallOnDeviceDialog diag = new InstallOnDeviceDialog( shell );
    diag.open();

  }
}
