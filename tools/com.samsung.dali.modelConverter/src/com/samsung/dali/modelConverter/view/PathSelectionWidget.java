package com.samsung.dali.modelConverter.view;

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

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Text;

public class PathSelectionWidget extends Composite {

	private Text textPath;
	private Button btnSelectPath;

	private String[] extensions;
	private int fileDialogStyle = SWT.OPEN;

	public PathSelectionWidget(Composite parent, int style, String defaultText) {
		super(parent, style);

		// create controls
		textPath = new Text(this, SWT.BORDER);
		textPath.setMessage(defaultText);

		btnSelectPath = new Button(this, SWT.BORDER);
		btnSelectPath.setText("Browse...");
		btnSelectPath.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				FileDialog dialog = new FileDialog(parent.getShell(), fileDialogStyle);
				dialog.setFilterPath(textPath.getText());
				if(extensions != null)
				{
					dialog.setFilterExtensions(extensions);
				}
				dialog.open();

				textPath.setText(dialog.getFilterPath() + "/" + dialog.getFileName());
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// TODO Auto-generated method stub

			}

		});

		// Do layouts
		GridLayout layout = new GridLayout();
		layout.numColumns = 2;
		setLayout(layout);

		GridData textData = new GridData();
		textData.widthHint = 600;
		textPath.setLayoutData(textData);
	}

	public void setFilterExtensions(String[] exts)
	{
		extensions = exts;
	}

	public void setFileDialogStyle(int style)
	{
		fileDialogStyle = style;
	}

	public Text getText() {
		return textPath;
	}
}
