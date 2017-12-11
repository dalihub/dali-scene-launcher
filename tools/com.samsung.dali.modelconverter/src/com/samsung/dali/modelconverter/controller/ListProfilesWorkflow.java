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

import java.util.List;

import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.view.parts.GlobalParts;
import com.samsung.dali.modelconverter.view.parts.OutputPart;

public class ListProfilesWorkflow {

  public static void execute(Shell shell, List<String> outProfiles) {
    assert outProfiles != null;

    OutputPart op = GlobalParts.getOutputPart();
    LoggingProcessRunner lpr = LoggingProcessRunner.create(shell.getDisplay(), op.getText());

    LoggingProcessRunner.Parser parser = new LoggingProcessRunner.Parser() {

      @Override
      public void parseLine(String line) {
        if (mCare) {
          if (!line.isEmpty()) {
            line = line.trim();
            // The profile might be active, in which case there will be a few spaces and an O following it. Then, go figure, more spaces.
            // Why is it an O? We don't know. Unfortunately, this means that your profile name, longer than 20 characters and ending in an O, will be truncated. But only slightly.
            if (line.endsWith("O") && line.length() >= 20) {
              line = line.substring(0, line.length() - 1).trim();
            }
            outProfiles.add(line);
          }
        }
        else {
          mCare = line.startsWith("[Profile Name]");
        }
      }

      private boolean mCare = false;
    };

    lpr.addCommand(GlobalData.get().getTizenPath() + " security-profiles list", parser)
      .run();
  }
}
