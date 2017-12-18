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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.List;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.graphics.Cursor;
import org.eclipse.swt.widgets.Display;

public class LoggingProcessRunner {

  public static abstract class Parser {
    public abstract void parseLine(String line);
  }

  public static LoggingProcessRunner create(Display display, StyledText text) {
    return new LoggingProcessRunner(display, text);
  }

  public LoggingProcessRunner addCommand(String cmd, Parser parser) {
    mCommands.add(new Job(cmd, parser));
    return this;
  }

  public LoggingProcessRunner addCommand(String cmd) {
    return addCommand(cmd, null);
  }

  public void run(boolean async) {
    if (mText != null) {
      mText.clearText();
    }

    if (mText != null && mCommands.isEmpty()) {
      try {
        mText.appendText("Nothing to do.", SWT.COLOR_GREEN);
      }
      catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
    }

    Runnable r = new Runnable() {

      @Override
      public void run() {

        Runtime rt = Runtime.getRuntime();
        List<Job> jobs = mCommands; // consume commands
        mCommands = null;

        int result = 0;
        for (Job job : jobs) {
          setBusy(true);

          try {
            if (mText != null) {
              mText.appendText(job.mCommand + "\n", SWT.COLOR_BLUE);
            }

            // Run process.
            Process proc = rt.exec(job.mCommand);

            // Get process output.
            try (BufferedReader in = new BufferedReader(new InputStreamReader(proc.getInputStream(), "UTF-8"))) {
              String line;
              while ((line = in.readLine()) != null) {

                if (job.mParser != null) {
                  job.mParser.parseLine(line);
                }

                if (mText != null) {
                  mText.appendText(line + "\n");
                }
              }
            }

            // Get process result.
            result = proc.waitFor();
            if (result != 0) {
              if (mText != null) {
                mText.appendText("'" + job.mCommand + "' failed.\n", SWT.COLOR_RED);
              }
              else {
                MessageDialog.openError(mDisplay.getActiveShell(), "Error", "'" + job.mCommand + "' failed.");
              }
              break;
            }
            else {
              if (mText != null) {
                mText.appendText("OK.\n", SWT.COLOR_DARK_GREEN);
              }
            }
          }
          catch (IOException | InterruptedException e) {
            try {
              if (mText != null) {
                mText.appendText(e.getMessage(), SWT.COLOR_RED);
              }
              else {
                MessageDialog.openError(mDisplay.getActiveShell(), "Error", e.getMessage());
              }
            }
            catch (IOException e1) {
              e1.printStackTrace();
            }
            break;
          }
          finally {
            setBusy(false);
          }
        }

        synchronized (this) {
          mResult = result;
        }
      }
    };

    if (async) {
      Thread t = new Thread(r);
      t.start();
    }
    else {
      r.run();
    }
  }

  public synchronized Integer getResult() {
    return mResult;
  }

  // private
  private static class Job {
    String mCommand;
    Parser mParser;

    Job(String cmd, Parser parser) {
      mCommand = cmd;
      mParser = parser;
    }
  }

  private Display mDisplay;
  private TextAppendHelper mText;
  private Cursor mCursor;

  private LinkedList<Job> mCommands = new LinkedList<Job>();

  private Integer mResult = 0;

  private LoggingProcessRunner(Display display, StyledText text) {
    mDisplay = display;

    if (text != null) {
      mText = new TextAppendHelper(text);
    }
  }

  private void setBusy(boolean isBusy) {
    if (!mDisplay.isDisposed()) {
      mDisplay.asyncExec(new Runnable() {

        @Override
        public void run() {

          int cursorType = isBusy ? SWT.CURSOR_WAIT : SWT.CURSOR_ARROW;
          Cursor cursor = new Cursor(mDisplay, cursorType);
          mDisplay.getActiveShell().setCursor(cursor);

          if (mCursor != null) {
            mCursor.dispose();
          }
          mCursor = cursor;
        }
      });
    }
  }
}
