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

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.widgets.Display;

public class TextAppendHelper {
  public TextAppendHelper(StyledText text) {
    mText = text;
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

  private void appendText(String str, StyleRange styleRange) {
    mText.setText(mText.getText() + str);
    mText.setTopIndex(mText.getLineCount());
    if (styleRange != null) {
      mText.setStyleRange(styleRange);
    }
  }

  private StyledText mText;
}
