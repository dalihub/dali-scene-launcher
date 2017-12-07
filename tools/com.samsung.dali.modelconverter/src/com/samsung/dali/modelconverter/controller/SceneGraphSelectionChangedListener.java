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

import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.widgets.TreeItem;

import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class SceneGraphSelectionChangedListener implements ISelectionChangedListener {

  SceneGraphSelectionChangedListener(Document doc, Property.Receiver receiver) {
    mDocument = doc;
    mReceiver = receiver;
  }

  @Override
  public void selectionChanged(SelectionChangedEvent event) {

    mReceiver.resetProperties();
    TreeViewer tv = (TreeViewer) event.getSource();
    TreeItem[] selection = tv.getTree().getSelection();
    if (selection.length == 1) {
      TreeItem ti = selection[0];
      ((Property.Provider) ti.getData()).provideProperties(mDocument, mReceiver);
    }
  }

  private Document mDocument;
  private Property.Receiver mReceiver;
}
