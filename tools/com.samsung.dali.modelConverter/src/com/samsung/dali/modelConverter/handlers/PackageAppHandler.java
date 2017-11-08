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
import java.util.ArrayList;
import java.util.List;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.LoggingProcessRunner;
import com.samsung.dali.modelConverter.view.PackageAppDialog;

public class PackageAppHandler {

	@Execute
	public void execute(IWorkbench workbench, Shell shell) throws InterruptedException {

		List<String> profiles;
		try
		{
			profiles = parseProfiles();

			PackageAppDialog dlg = new PackageAppDialog(shell, profiles);
			if(dlg.open() == Window.OK)
			{
				String tizenCmd = "tizen";
				if(System.getProperty("os.name").toLowerCase().contains("windows"))
				{
					tizenCmd += ".bat";
				}

				String projectPath = dlg.getProjectPath();
				String buildConfig = dlg.getBuildConfig();
				String securityProfile = dlg.getSecurityProfile();

				LoggingProcessRunner lpr = LoggingProcessRunner.create();
				if(!dlg.skipBuildingRequested())
				{
					lpr.addCommand(tizenCmd + " build-native -a arm -c gcc -C " + buildConfig + " -- " + projectPath);
				}
				lpr.addCommand(tizenCmd + " package -t tpk -s " + securityProfile + " -- " + projectPath + "/" + buildConfig)
					.run();
			}
		}
		catch (IOException e)
		{
			MessageBox mb = new MessageBox(shell, SWT.ERROR);
			mb.setText("Error");
			mb.setMessage("Failed to get list of security profiles. Have you got Tizen Studio installed and in your PATH?");
			mb.open();
		}
	}

	List<String> parseProfiles() throws IOException, InterruptedException
	{
		List<String> profiles = new ArrayList<String>();
		Runtime rt = Runtime.getRuntime();

		Process proc = rt.exec("tizen.bat security-profiles list");
		boolean care = false;
		try (BufferedReader in = new BufferedReader(new InputStreamReader(proc.getInputStream(), "UTF-8"))) {
			while (proc.isAlive()) {
				String line = in.readLine();
				if (line == null) {
					break;
				}

				System.out.println(line);

				if(!care)
				{
					care = line.startsWith("[Profile Name]");
				}
				else
				{
					line = line.split("[ \t]")[0];
					profiles.add(line);
				}
			}
		}

		proc.waitFor();
		return profiles;
	}
}
