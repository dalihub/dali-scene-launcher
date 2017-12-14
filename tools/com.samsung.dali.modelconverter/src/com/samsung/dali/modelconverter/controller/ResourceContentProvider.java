package com.samsung.dali.modelconverter.controller;

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

import org.eclipse.jface.viewers.ITreeContentProvider;
import java.util.ArrayList;

/*
 * Provides descriptions of a given list of resources.
 */
public class ResourceContentProvider<T> implements ITreeContentProvider {

  public ResourceContentProvider(ArrayList<T> resources) {
    mResources = resources;
  }

  /*
   * Return the list of resources.
   */
  @Override
  public Object[] getElements(Object inputElement) {
    return mResources.toArray();
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

  private ArrayList<T> mResources;
}
