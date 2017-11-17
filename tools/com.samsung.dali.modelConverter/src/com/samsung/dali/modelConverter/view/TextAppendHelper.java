package com.samsung.dali.modelConverter.view;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.widgets.Display;

public class TextAppendHelper {
  public TextAppendHelper(StyledText text)
  {
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
