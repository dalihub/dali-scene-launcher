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

import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;

public class SceneGraphPart {

  @PostConstruct
  public void createComposite(Composite parent) {
    parent.setLayout(new FormLayout());

    TreeViewer treeViewer = new TreeViewer(parent, SWT.BORDER);
    Tree tree = treeViewer.getTree();
    FormData fd_tree = new FormData();
    fd_tree.bottom = new FormAttachment(100, -10);
    fd_tree.right = new FormAttachment(100, -5);
    fd_tree.top = new FormAttachment(0, 5);
    fd_tree.left = new FormAttachment(0, 5);
    tree.setLayoutData(fd_tree);

    sActiveInstance = this;
  }

  static SceneGraphPart sActiveInstance;

}