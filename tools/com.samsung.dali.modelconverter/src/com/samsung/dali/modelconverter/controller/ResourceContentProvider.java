package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;

/*
 * Provides descriptions of a given list of resources.
 */
public class ResourceContentProvider<T> implements ITreeContentProvider {

  public ResourceContentProvider(Document doc) {
    mDocument = doc;
  }

  public Object getDocument() {
    return mDocument;
  }

  /*
   * Return the list of resources.
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
}
