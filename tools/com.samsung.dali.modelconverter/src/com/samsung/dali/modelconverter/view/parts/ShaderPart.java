package com.samsung.dali.modelconverter.view.parts;

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

import javax.annotation.PostConstruct;

import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;

import com.samsung.dali.modelconverter.controller.PropertyProviderSelectionChangedListener;
import org.eclipse.jface.viewers.ITreeContentProvider;

public class ShaderPart {

  public static final String sId = "com.samsung.dali.modelconverter.part.shaders";

  @PostConstruct
  public void createComposite(Composite parent) {

    mTreeViewer = new TreeViewer(parent, SWT.BORDER);
    mTree = mTreeViewer.getTree();
  }

  public void populate(ITreeContentProvider provider, PropertyProviderSelectionChangedListener listener) {
    mTree.removeAll();

    if (mSelectionChangedListener != null) {
      mTreeViewer.removeSelectionChangedListener(mSelectionChangedListener);
    }
    mTreeViewer.addSelectionChangedListener(listener);

    mTreeViewer.setContentProvider(provider);
    mTreeViewer.setInput(provider);
    mTreeViewer.refresh();
  }

  private TreeViewer mTreeViewer;
  private Tree mTree;
  private ISelectionChangedListener mSelectionChangedListener;
}