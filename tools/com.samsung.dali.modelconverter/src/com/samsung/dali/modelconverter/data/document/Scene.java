package com.samsung.dali.modelconverter.data.document;

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

import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonSetter;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class Scene implements Property.Provider {

  @Override
  public String toString() {
    String name = "Scene " + mId;
    if (mIsOrphan) {
      name += " [ORPHAN]";
    }
    return name;
  }

  @JsonIgnore
  public int getId() {
    return mId;
  }

  public void setId(int id) {
    mId = id;
  }

  @JsonIgnore
  public boolean isOrphan() {
    return mIsOrphan;
  }

  public void setOrphan(boolean isIt) {
    mIsOrphan = isIt;
  }

  @JsonIgnore
  public Integer getRootId() {
    return mRootId;
  }

  public void setRootId(Integer rootId) {
    mRootId = rootId;
  }

  @JsonSetter("nodes")
  public void setNodes(ArrayList<Integer> nodes) {
    if (nodes.size() != 1) {
      throw new IllegalArgumentException("Scene.nodes must be an array of a single node index. Sorry about that.");
    }
    mRootId = nodes.get(0).intValue();
  }

  @JsonGetter("nodes")
  public ArrayList<Integer> getNodes() {
    ArrayList<Integer> nodes = new ArrayList<Integer>();
    nodes.add(new Integer(mRootId));
    return nodes;
  }

  @Override
  public void provideProperties(Document context, Property.Receiver receiver) {
    // nothing.
  }

  private int mId = -1;
  private boolean mIsOrphan = false;
  private Integer mRootId = -1;
}
