package com.samsung.dali.modelConverter.parts;

import java.util.ArrayList;

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

import org.eclipse.jface.viewers.IContentProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;
import com.samsung.dali.modelConverter.data.GlobalData;
import com.samsung.dali.modelConverter.data.Project;
import com.samsung.dali.modelConverter.data.dlimodel.*;

public class NodeTreePart {
  private Tree mTree;
  private TreeViewer mTreeViewer;

  @PostConstruct
  public void createControls(Composite parent) {
    GlobalData.Get().setNodeTreePart( this );
    mTreeViewer = new TreeViewer(parent, SWT.BORDER);
    mTreeViewer.setContentProvider((IContentProvider) new NodeProvider());
    
    mTree = mTreeViewer.getTree();

  }

  public void resetTree() {
    
    mTree.removeAll();
  }

  public void setData() {
    resetTree();
    mTreeViewer.setInput(Project.sInstance.getDliModel());
  }

  public class NodeProvider implements ITreeContentProvider
  {
    @Override
    public Object[] getElements(Object inputElement) {

      return getChildren(inputElement);
    }

    @Override
    public Object[] getChildren(Object parentElement) {

      if(parentElement instanceof Node)
      {
        Node n = (Node)parentElement;
        return n.children.toArray();
      }
      else if(parentElement instanceof DliModel)
      {
        DliModel d = (DliModel)parentElement;
        ArrayList<Node> nodes = new ArrayList<Node>();
        for(Scene s : d.scenes)
        {
          if(s.root != null)
          {
            nodes.add(s.root);
          }
        }
        return nodes.toArray();
      }
      else
      {
        throw new IllegalArgumentException("Type " + Object.class.getName() + " is invalid in scene graph.");
      }
    }

    @Override
    public Object getParent(Object element) {
      Node n = (Node)element;
      return n;
    }

    @Override
    public boolean hasChildren(Object element) {
      Node n = (Node)element;
      return !n.children.isEmpty();
    }
    
  }

}
