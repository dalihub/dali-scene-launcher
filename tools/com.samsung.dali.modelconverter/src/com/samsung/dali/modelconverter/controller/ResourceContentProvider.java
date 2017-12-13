package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;

/*
 * Provides descriptions of a given list of resources.
 */
public class ResourceContentProvider<T> implements ITreeContentProvider {

  public ResourceContentProvider(T[] resources) {
    mResources = resources;
  }

  /*
   * Return the list of resources.
   */
  @Override
  public Object[] getElements(Object inputElement) {
    return mResources;
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

  private T[] mResources;
}
