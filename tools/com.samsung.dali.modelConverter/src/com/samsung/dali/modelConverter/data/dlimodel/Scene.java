package com.samsung.dali.modelConverter.data.dlimodel;

import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonSetter;

public class Scene implements DliModel.IdResolver {

  @JsonIgnore
  public int rootId = -1;
  
  @JsonIgnore
  public Node root;

  @JsonSetter("nodes")
  public void setNodes(ArrayList<Integer> nodes)
  {
    if (nodes.size() != 1)
    {
      throw new IllegalArgumentException("Scene.nodes must be an array of a single node index. Sorry about that.");
    }
    rootId = nodes.get(0).intValue();
  }
  
  @JsonGetter("nodes")
  public ArrayList<Integer> getNodes()
  {
    ArrayList<Integer> nodes = new ArrayList<Integer>();
    nodes.add(new Integer(rootId));
    return nodes;
  }

  @Override
  public void resolveIds(DliModel doc) {
    
    root = doc.nodes.get(rootId); 
  }

  @Override
  public void updateIds(DliModel doc) {
    
    rootId = doc.nodes.indexOf(root);
  }
}
