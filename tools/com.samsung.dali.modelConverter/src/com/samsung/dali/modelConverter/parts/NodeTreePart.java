package com.samsung.dali.modelConverter.parts;

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

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;

import com.samsung.dali.modelConverter.data.GlobalData;

public class NodeTreePart {
  @PostConstruct
  public void createComposite(Composite parent) {
    GlobalData.Initialize(parent.getShell());
    parent.setLayout(new FillLayout(SWT.HORIZONTAL));

    Tree tree = new Tree(parent, SWT.BORDER);

    TreeItem itemN1 = new TreeItem(tree, SWT.NONE);
    itemN1.setText("Node1");

    TreeItem sItem1 = new TreeItem(itemN1, SWT.NONE);
    sItem1.setText("subNode1");
    itemN1.setExpanded(true);

    TreeItem itemN2 = new TreeItem(tree, SWT.NONE);
    itemN2.setText("Node2");

  }
}
