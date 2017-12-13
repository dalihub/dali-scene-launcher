package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;

/*
 * Provides descriptions of the meshes.
 */
public class ResourceContentProvider implements ITreeContentProvider {

  public ResourceContentProvider(Document doc, Class<?> type) {
    mDocument = doc;
    mType = type;
  }

  public Object getDocument() {
    return mDocument;
  }

  /*
   * Get the top level nodes from an element, which should only be the Document
   * that the provider was created with. The nodes are meshes.
   */
  @Override
  public Object[] getElements(Object inputElement) {
    assert inputElement == mDocument;

    ArrayList<Object> kids = new ArrayList<Object>();

    return kids.toArray();
  }

  @Override
  public Object[] getChildren(Object parentElement) {
    return null;
  }

  @Override
  public Object getParent(Object element) {
    return null;
  }

  @Override
  public boolean hasChildren(Object element) {
    return false;
  }

  private Document mDocument;
  private Class<?> mType;
}
