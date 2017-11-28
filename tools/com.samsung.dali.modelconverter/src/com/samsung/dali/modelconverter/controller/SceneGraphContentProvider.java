package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.Node;

public class SceneGraphContentProvider implements ITreeContentProvider {

  public SceneGraphContentProvider(Document doc) {
    mDocument = doc;
  }

  public Object getDocument() {
    return mDocument;
  }

  @Override
  public Object[] getElements(Object inputElement) {
    return getChildren(inputElement);
  }

  @Override
  public Object[] getChildren(Object parentElement) {
    ArrayList<Object> kids = new ArrayList<Object>();
    if (parentElement == mDocument) {
      kids.addAll(mDocument.getRootNodes());
      kids.addAll(mDocument.getCameras());

      if (kids.isEmpty()) {
        kids.add("<The document is empty>");
      }
    }
    else if (parentElement instanceof Node) {
      kids.addAll(mDocument.getNodeChildren((Node) parentElement));
    }
    return kids.toArray();
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

    return getChildren(element).length > 0;
  }

  private Document mDocument;
}
