package com.samsung.dali.modelConverter;

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

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.LinkedList;
import java.util.List;

import org.eclipse.e4.core.contexts.EclipseContextFactory;
import org.eclipse.e4.core.contexts.IEclipseContext;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.swt.SWT;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.FrameworkUtil;

public class LoggingProcessRunner {

	public static LoggingProcessRunner create()
	{
		return new LoggingProcessRunner();
	}

	public LoggingProcessRunner addCommand(String cmd)
	{
		mCommands.add(cmd);
		return this;
	}

	public void run()
	{
		// Get MainPart for dumping text onto it.
		Bundle bundle = FrameworkUtil.getBundle(EPartService.class);
		BundleContext bundleContext = bundle.getBundleContext();
		IEclipseContext eclipseCtx = EclipseContextFactory.getServiceContext(bundleContext);
		EPartService service = (EPartService)eclipseCtx.get(EPartService.class.getName());
		final MainPart mainPart = (MainPart) service.findPart("com.samsung.dali.modelConverter.part.Main").getObject();

		// Clear text.
		mainPart.clearText();

		if(mCommands.isEmpty())
		{
			try {
				mainPart.appendText("Nothing to do.", SWT.COLOR_GREEN);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		Runnable r = new Runnable() {

			@Override
			public void run() {

				Runtime rt = Runtime.getRuntime();
				List<String> commands = mCommands;	// consume commands
				mCommands = null;

				for(String cmd: commands)
				{
					mainPart.setBusy(true);
					try {
						mainPart.appendText(cmd + "\n", SWT.COLOR_BLUE);

						// Run process.
						Process proc = rt.exec(cmd);

						// Get process output. NOTE: there's some scope here for parsing output.
						char[] buffer = new char[1024];
						try (Reader in = new InputStreamReader(proc.getInputStream(), "UTF-8")) {
							while (proc.isAlive()) {
								int rsz = in.read(buffer, 0, buffer.length);
								if (rsz < 0) {
									break;
								}

								String text = String.copyValueOf(buffer, 0, rsz);
								text = text.replaceAll("\r\r", "\r");	// Workaround for double line breaks.
								mainPart.appendText(text);
							}
						}

						// Get process result.
						int result = proc.waitFor();
						if(result == 0)
						{
							mainPart.appendText("OK.\n", SWT.COLOR_DARK_GREEN);
						}
						else
						{
							mainPart.appendText("'" + cmd + "' failed.\n", SWT.COLOR_RED);
							break;
						}
					} catch (IOException e) {
						try {
							mainPart.appendText(e.getMessage(), SWT.COLOR_RED);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						break;
					} catch (InterruptedException e) {
						try {
							mainPart.appendText(e.getMessage(), SWT.COLOR_RED);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						break;
					}
					finally {
						mainPart.setBusy(false);
					}
				}
			}
		};

		Thread t = new Thread(r);
		t.start();
	}

	// private
	private LinkedList<String> mCommands;

	private LoggingProcessRunner()
	{
		mCommands = new LinkedList<String>();
	}
}

