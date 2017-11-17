package com.samsung.dali.modelConverter.process;

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

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.graphics.Cursor;
import org.eclipse.swt.widgets.Display;

import com.samsung.dali.modelConverter.view.TextAppendHelper;

public class LoggingProcessRunner {

  public static LoggingProcessRunner create(Display display, StyledText text)
  {
    return new LoggingProcessRunner(display, text);
  }

  public LoggingProcessRunner addCommand(String cmd)
  {
    mCommands.add(cmd);
    return this;
  }

  public void run()
  {
    mText.clearText();

    if(mCommands.isEmpty())
    {
      try {
        mText.appendText("Nothing to do.", SWT.COLOR_GREEN);
      } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
    }

    Runnable r = new Runnable() {

      @Override
      public void run() {

        Runtime rt = Runtime.getRuntime();
        List<String> commands = mCommands;  // consume commands
        mCommands = null;

        for(String cmd: commands)
        {
          setBusy(true);
          try {
            mText.appendText(cmd + "\n", SWT.COLOR_BLUE);

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
                text = text.replaceAll("\r\r", "\r");  // Workaround for double line breaks.
                mText.appendText(text);
              }
            }

            // Get process result.
            int result = proc.waitFor();
            if(result == 0)
            {
              mText.appendText("OK.\n", SWT.COLOR_DARK_GREEN);
            }
            else
            {
              mText.appendText("'" + cmd + "' failed.\n", SWT.COLOR_RED);
              break;
            }
          } catch (IOException e) {
            try {
              mText.appendText(e.getMessage(), SWT.COLOR_RED);
            } catch (IOException e1) {
              e1.printStackTrace();
            }
            break;
          } catch (InterruptedException e) {
            try {
              mText.appendText(e.getMessage(), SWT.COLOR_RED);
            } catch (IOException e1) {
              e1.printStackTrace();
            }
            break;
          }
          finally {
            setBusy(false);
          }
        }
      }
    };

    Thread t = new Thread(r);
    t.start();
  }

  // private
  private Display mDisplay;
  private TextAppendHelper mText;
  private Cursor mCursor;
  
  private LinkedList<String> mCommands;

  private LoggingProcessRunner(Display display, StyledText text)
  {
    mDisplay = display;
    mText = new TextAppendHelper(text);
    mCommands = new LinkedList<String>();
  }
  
  private void setBusy(boolean isBusy)
  {
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

