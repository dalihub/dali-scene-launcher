package com.samsung.dali.modelconverter.data.document;

import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonSetter;

public class Scene {

  @Override
  public String toString() {
    String name = "Scene " + mId;
    if (mIsOrphan) {
      name += " [ORPHAN]";
    }
    return name;
  }

  public int getId() {
    return mId;
  }

  public void setId(int id) {
    mId = id;
  }

  public boolean isOrphan() {
    return mIsOrphan;
  }

  public void setOrphan(boolean isIt) {
    mIsOrphan = isIt;
  }

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

  @JsonIgnore
  private int mId = -1;

  @JsonIgnore
  private boolean mIsOrphan = false;

  @JsonIgnore // custom setter / getter provided -- json representation is an array of a
              // single integer element.
  private Integer mRootId = -1;
}
