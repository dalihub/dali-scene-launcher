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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.SettingsClass;
import com.samsung.dali.modelConverter.windows.SelectCertificateDialog;

public class SelectCertificateHandler {
  @Execute
  public void execute(Shell shell) throws InterruptedException {
    List<String> profiles;
    try
    {
      profiles = parseProfiles();
      SelectCertificateDialog dlg = new SelectCertificateDialog(shell, profiles);
      dlg.open();

    }
    catch (IOException e)
    {
      SettingsClass.CheckTizenCliPath( shell, true );
    }
  }

  List<String> parseProfiles() throws IOException, InterruptedException
  {
    List<String> profiles = new ArrayList<String>();
    Runtime rt = Runtime.getRuntime();
    String cmd = GlobalData.Get().GetTizenPath() + "tizen security-profiles list";
    if(GlobalData.Get().IsWindows())
    {
      cmd = GlobalData.Get().GetTizenPath() + "tizen.bat security-profiles list";
    }

    Process proc = rt.exec(cmd);
    boolean care = false;
    try (BufferedReader in = new BufferedReader(new InputStreamReader(proc.getInputStream(), "UTF-8"))) {
      while (true) {
        String line = in.readLine();
        if ((line == null) && proc.isAlive() ) {
          continue;
        }
        if ((line == null) && !proc.isAlive() ) {
          break;
        }

        System.out.println(line);

        if(!care)
        {
          care = line.startsWith("[Profile Name]");
        }
        else
        {
          line = line.split("[   ]")[0];
          profiles.add(line);
        }

      }
    }

    proc.waitFor();
    return profiles;
  }
}
