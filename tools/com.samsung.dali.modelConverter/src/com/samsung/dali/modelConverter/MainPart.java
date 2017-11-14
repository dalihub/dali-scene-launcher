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

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

import javax.annotation.PostConstruct;

import org.eclipse.e4.ui.di.Focus;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Cursor;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelConverter.model.ModelConverterLogic;
import com.samsung.dali.modelConverter.view.PathSelectionWidget;

public class MainPart {

	@PostConstruct
	public void createControls(Composite parent) {

		// Create controls
		mPswImport = new PathSelectionWidget(parent, 0, "Select file to import");
		mPswImport.setFilterExtensions(new String[] { "*.dae" });

		mPswExport = new PathSelectionWidget(parent, 0, "Select file to export to");
		mPswExport.setDialogStyle(SWT.SAVE);

		mBtnDoExport = new Button(parent, 0);
		mBtnDoExport.setText("Perform conversion");
		mBtnDoExport.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				Text importText = mPswImport.getText();
				Text exportText = mPswExport.getText();

				// perform conversion
				ModelConverterLogic model = ModelConverterLogic.getInstance();
				model.setImportPath(importText.getText());
				model.setExportPath(exportText.getText());

				String dli = model.performConversion();
				if (dli != null) {
					String dliName = exportText.getText() + ".dli";
					
					MessageBox mb = new MessageBox(parent.getShell());
					mb.setText("Conversion successful");
					mb.setMessage("File saved to " + dliName + " and .bin.");
					mb.open();

					mText.setText(dli.toString());

					File dliFile = new File(dliName);
					try (FileWriter dliWriter = new FileWriter(dliFile)) {
						dliWriter.write(dli);
					} catch (IOException e1) {
					}
				}
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// TODO Auto-generated method stub

			}

		});

		mText = new StyledText(parent, SWT.BORDER | SWT.MULTI | SWT.V_SCROLL);
		mText.setAlwaysShowScrollBars(false);
		mText.setWordWrap(true);

		// do layout
		FormLayout layout = new FormLayout();
		parent.setLayout(layout);

		FormData importData = new FormData();
		importData.top = new FormAttachment(0, 0);
		mPswImport.setLayoutData(importData);

		FormData exportData = new FormData();
		exportData.top = new FormAttachment(mPswImport, 0);
		mPswExport.setLayoutData(exportData);

		FormData btnData = new FormData();
		btnData.top = new FormAttachment(mPswExport, 0);
		btnData.right = new FormAttachment(100, 0);
		mBtnDoExport.setLayoutData(btnData);

		FormData textData = new FormData();
		textData.left = new FormAttachment(0, 0);
		textData.right = new FormAttachment(100, 0);
		textData.top = new FormAttachment(mBtnDoExport, 0);
		textData.bottom = new FormAttachment(100, 0);
		mText.setLayoutData(textData);
	}

	public void setBusy(boolean isBusy) {
		Display display = mText.getDisplay();
		if (!display.isDisposed()) {
			display.asyncExec(new Runnable() {

				@Override
				public void run() {
					// update controls -- note that the busy cursor won't show over enabled text
					// fields (which is most of the UI).
					boolean isActive = !isBusy;
					mPswImport.setEnabled(isActive);
					mPswExport.setEnabled(isActive);
					mBtnDoExport.setEnabled(isActive);
					mText.setEnabled(isActive);

					int cursorType = isBusy ? SWT.CURSOR_WAIT : SWT.CURSOR_ARROW;
					Cursor cursor = new Cursor(display, cursorType);
					mText.getShell().setCursor(cursor);

					if (mCursor != null) {
						mCursor.dispose();
					}
					mCursor = cursor;
				}
			});
		}

	}

	public void clearText() {
		Display display = mText.getDisplay();
		if (!display.isDisposed()) {
			display.asyncExec(new Runnable() {

				@Override
				public void run() {
					mText.setText("");
				}
			});
		}
	}

	public void appendText(String str) throws UnsupportedEncodingException, IOException {
		appendText(str, SWT.NONE);
	}

	public void appendText(String str, int color) throws UnsupportedEncodingException, IOException {
		Display display = mText.getDisplay();
		if (!display.isDisposed()) {
			display.asyncExec(new Runnable() {
				public void run() {
					Color swtColor = null;
					if (color != SWT.NONE) {
						swtColor = display.getSystemColor(color);
					}

					StyleRange styleRange = null;
					if (swtColor != null) {
						styleRange = new StyleRange(mText.getText().length(), str.length(), swtColor, null);
					}
					appendText(str, styleRange);
				}
			});
		}
	}

	public void appendText(String str, StyleRange styleRange) {
		mText.setText(mText.getText() + str);
		mText.setTopIndex(mText.getLineCount());
		if (styleRange != null) {
			mText.setStyleRange(styleRange);
		}
	}

	public String getText() {
		return mText.getText();
	}

	@Focus
	public void setFocus() {
		mPswImport.getText().setFocus();
	}

	// private
	private PathSelectionWidget mPswImport;
	private PathSelectionWidget mPswExport;
	private Button mBtnDoExport;
	private StyledText mText;

	private Cursor mCursor;

}
