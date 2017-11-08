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

import java.io.File;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.workbench.IWorkbench;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.LoggingProcessRunner;

public class DeployRpmHandler {

	@Execute
	public void execute(IWorkbench workbench, Shell shell) {

		FileDialog dialog = new FileDialog(shell);
		String[] filterExtensions = new String[] { "*.rpm" };
		dialog.setFilterExtensions(filterExtensions);
		String rpmPath = dialog.open();
		if(rpmPath == null)
		{
			return;
		}

		LoggingProcessRunner.create()
			.addCommand("sdb root on")
			.addCommand("sdb shell change-booting-mode.sh --update")
			.addCommand("sdb shell push " + rpmPath + " /tmp/")
			.addCommand("sdb shell rpm --force -U /tmp/" + new File(rpmPath).getName())
			.run();
	}
}
