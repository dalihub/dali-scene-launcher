package com.samsung.dali.modelconverter.data.document;

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
