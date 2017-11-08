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

import java.util.List;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.SelectionAdapter;

public class PackageAppDialog extends Dialog {

	public PackageAppDialog(Shell parent, List<String> profiles) {
		super(parent);
		setShellStyle(SWT.SYSTEM_MODAL);
		mProfiles = profiles;
	}

	@Override
	protected void configureShell(Shell newShell) {
		super.configureShell(newShell);
		newShell.setText("Package App");
	}

	@Override
	protected Point getInitialSize() {
		return new Point(500, 200);
	}

	@Override
	protected Control createDialogArea(Composite parent) {

		Composite container = (Composite) super.createDialogArea(parent);
		GridLayout gl_container = new GridLayout(2, false);
		gl_container.horizontalSpacing = 1;
		container.setLayout(gl_container);

		// path selection widget
		PathSelectionWidget pswProjectPath = new PathSelectionWidget(container, SWT.NONE, "Browse to Tizen Studio project");
		GridData gd_pswProjectPath = new GridData(SWT.FILL, SWT.CENTER, false, false, 2, 1);
		gd_pswProjectPath.heightHint = 39;
		pswProjectPath.setLayoutData(gd_pswProjectPath);
		GridLayout gridLayout = (GridLayout) pswProjectPath.getLayout();
		gridLayout.horizontalSpacing = 1;
		pswProjectPath.setDirectory(true);
		pswProjectPath.getText().addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(ModifyEvent e) {
				mProjectPath = ((Text) e.widget).getText();
			}
		} );

		Label lblBuildConfig = new Label(container, SWT.NONE);
		lblBuildConfig.setText("Build configuration:");

		Combo cbxBuildConfig = new Combo(container, SWT.NONE);
		GridData gd_cbxBuildConfig = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
		gd_cbxBuildConfig.widthHint = 340;
		cbxBuildConfig.setLayoutData(gd_cbxBuildConfig);
		cbxBuildConfig.setItems(sBuildConfigs);	// TODO: read from project somehow?
		cbxBuildConfig.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				Combo c = ((Combo) e.widget);
				mBuildConfig = c.getItem(c.getSelectionIndex());
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
			}

		});

		if(cbxBuildConfig.getItemCount() > 0)
		{
			cbxBuildConfig.select(0);
			mBuildConfig = cbxBuildConfig.getItems()[0];
		}

		Button btnSkipBuilding = new Button(container, SWT.CHECK);
		btnSkipBuilding.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				mSkipBuildingRequested = ((Button)e.widget).getSelection();
			}
		});
		btnSkipBuilding.setLayoutData(new GridData(SWT.LEFT, SWT.CENTER, false, false, 2, 1));
		btnSkipBuilding.setText("Skip building");

		Label lblSecurityProfile = new Label(container, SWT.FILL);
		lblSecurityProfile.setText("Security profile:");

		Combo cbxSecurityProfile = new Combo(container, SWT.FILL);
		GridData gd_cbxSecurityProfile = new GridData(SWT.LEFT, SWT.CENTER, false, false, 1, 1);
		gd_cbxSecurityProfile.widthHint = 340;
		cbxSecurityProfile.setLayoutData(gd_cbxSecurityProfile);
		cbxSecurityProfile.setItems(mProfiles.toArray(new String[0]));
		cbxSecurityProfile.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				Combo c = ((Combo) e.widget);
				mSecurityProfile = c.getItem(c.getSelectionIndex());
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
			}
		});

		if(cbxSecurityProfile.getItemCount() > 0)
		{
			cbxSecurityProfile.select(0);
			mSecurityProfile = cbxSecurityProfile.getItems()[0];
		}

		return container;
	}

	public String getProjectPath() {
		return mProjectPath;
	}

	public boolean skipBuildingRequested() {
		return mSkipBuildingRequested;
	}

	public String getBuildConfig() {
		return mBuildConfig;
	}

	public String getSecurityProfile() {
		return mSecurityProfile;
	}

	private static String[] sBuildConfigs = { "Debug", "Release" };

	private List<String> mProfiles;

	private String mProjectPath;
	private String mBuildConfig;
	private boolean mSkipBuildingRequested = false;
	private String mSecurityProfile;
}
