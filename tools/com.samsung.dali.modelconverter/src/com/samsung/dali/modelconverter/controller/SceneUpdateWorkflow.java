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

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.samsung.dali.modelconverter.data.GlobalData;
import com.samsung.dali.modelconverter.data.Project;
import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.view.parts.GlobalParts;
import com.samsung.dali.modelconverter.view.parts.NodePropertiesPart;

/*
 * Gets the document from the current project and updates the SceneGraph & Dli views.
 */
public class SceneUpdateWorkflow {

  public static void execute(Shell shell) {

    GlobalData globalData = GlobalData.get();
    Project project = globalData.getProject();
    Document doc = project.getDocument();
    SceneGraphContentProvider provider = new SceneGraphContentProvider(doc);

    NodePropertiesPart npp = GlobalParts.getNodePropertiesPart();
    NodePropertyReceiver receiver = new NodePropertyReceiver(npp);
    SceneGraphSelectionChangedListener selectionChangedListener = new SceneGraphSelectionChangedListener(doc, receiver);

    GlobalParts.getSceneGraphPart().populate(provider, selectionChangedListener);

    String dli = "<ERROR>";
    try {
      dli = doc.toDliString();
    }
    catch (JsonProcessingException e) {
      MessageDialog.openError(shell, "Error opening scene.", e.getMessage());
    }

    GlobalParts.getDliTextPart().populate(dli);
  }
}
