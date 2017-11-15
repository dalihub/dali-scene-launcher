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

public class LoggingProcessRunner {

  ILogText mLogText;
  // private
  private LinkedList<String> mCommands;

  private LoggingProcessRunner( ILogText itext )
  {
    mLogText = itext;
    mCommands = new LinkedList<String>();
  }

  public static LoggingProcessRunner create( ILogText itext )
  {
    return new LoggingProcessRunner( itext );
  }

  public LoggingProcessRunner addCommand(String cmd)
  {
    mCommands.add(cmd);
    return this;
  }

  public void run() throws IOException
  {
    // Clear text.
    if( mLogText != null ) {
      mLogText.ClearText();
    }

    if(mCommands.isEmpty())
    {
      if( mLogText != null ) {
        mLogText.AppendText( "Nothing to do.", SWT.COLOR_GREEN );
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
          mLogText.SetBusy( true );

          try {
            mLogText.AppendText( cmd + "\n", SWT.COLOR_BLUE );

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
                text = text.replaceAll("\r\r", "\r"); // Workaround for double line breaks.
                mLogText.AppendText(text, SWT.NONE );
              }
            }

            // Get process result.
            int result = proc.waitFor();
            if(result == 0)
            {
              mLogText.AppendText( "OK.\n", SWT.COLOR_DARK_GREEN );
            }
            else
            {
              mLogText.AppendText( "'" + cmd + "' failed.\n", SWT.COLOR_RED );
              break;
            }
          }
          catch (IOException e) {
            mLogText.AppendText(e.getMessage(), SWT.COLOR_RED);
            break;
          }
          catch (InterruptedException e) {
            mLogText.AppendText(e.getMessage(), SWT.COLOR_RED);
            break;
          }
          finally {
            mLogText.SetBusy( false );
          }
        }
      }
    };

    Thread t = new Thread(r);
    t.start();
  }


}
