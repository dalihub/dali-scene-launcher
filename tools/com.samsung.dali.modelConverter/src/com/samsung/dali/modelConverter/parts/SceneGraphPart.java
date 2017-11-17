package com.samsung.dali.modelConverter.parts;

import java.util.ArrayList;

import javax.annotation.PostConstruct;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;

import com.samsung.dali.modelConverter.model.document.Document;
import com.samsung.dali.modelConverter.model.document.Node;
import com.samsung.dali.modelConverter.model.document.Scene;

public class SceneGraphPart {

  public static SceneGraphPart sActiveInstance;
  
  @PostConstruct
  public void createControls(Composite parent) {
    
    mTreeViewer = new TreeViewer(parent, SWT.BORDER);
    mTreeViewer.setContentProvider(new NodeProvider());
    
    mTree = mTreeViewer.getTree();

    sActiveInstance = this;
  }

  public void resetTree() {
    
    mTree.removeAll();
  }

  public void populate(Document document) {
    resetTree();
    mTreeViewer.setInput(document);
  }
  
  private Tree mTree;
  private TreeViewer mTreeViewer;

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
      else if(parentElement instanceof Document)
      {
        Document d = (Document)parentElement;
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
