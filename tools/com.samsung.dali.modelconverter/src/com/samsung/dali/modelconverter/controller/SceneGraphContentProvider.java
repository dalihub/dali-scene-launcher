package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.Node;

/*
 * Provides nodes to the scene graph, which may be Nodes or Cameras of a Document.
 * Only the Nodes may have child nodes.
 */
public class SceneGraphContentProvider implements ITreeContentProvider {

  public SceneGraphContentProvider(Document doc) {
    mDocument = doc;
  }

  public Object getDocument() {
    return mDocument;
  }

  /*
   * Get the top level nodes from an element, which should only be the
   * Document that the provider was created with. The nodes may be
   * [a Scene's root] Nodes, or Cameras.
   */
  @Override
  public Object[] getElements(Object inputElement) {
    assert inputElement == mDocument;

    ArrayList<Object> kids = new ArrayList<Object>();
    kids.addAll(mDocument.getRootNodes());
    kids.addAll(mDocument.getCameras());

    if (kids.isEmpty()) {
      kids.add("<The document is empty>");
    }

    return kids.toArray();
  }

  /*
   * Get children of an element, which may only be a Node.
   */
  @Override
  public Object[] getChildren(Object parentElement) {

    Object[] kids = null;
    if (parentElement instanceof Node) {
      kids = mDocument.getNodeChildren((Node) parentElement).toArray();
    }
    return kids;
  }

  @Override
  public Object getParent(Object element) {

    if (element instanceof Node) {
      return ((Node) element).getParent();
    }
    return null;
  }

  @Override
  public boolean hasChildren(Object element) {

    Object[] kids = getChildren(element);
    return kids != null && kids.length > 0;
  }

  private Document mDocument;
}
