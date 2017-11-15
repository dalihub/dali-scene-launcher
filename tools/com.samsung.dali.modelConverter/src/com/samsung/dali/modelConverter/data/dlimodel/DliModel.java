package com.samsung.dali.modelConverter.data.dlimodel;

import java.util.ArrayList;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;

/*
 * @brief A dli document, serialisable to / from JSON (glTF).
 * Capable of resolving internal references.
 */
public class DliModel {

  public interface IdResolver
  {
    public abstract void resolveIds(DliModel doc);
    public abstract void updateIds(DliModel doc);
  }
  
  @JsonIgnoreProperties(ignoreUnknown = true)

  @JsonProperty("asset")
  public Asset asset;
  
  @JsonProperty("scene")
  public int defaultSceneId;
  
  @JsonIgnore
  public Scene defaultScene;
  
  @JsonProperty("scenes")
  public ArrayList<Scene> scenes;
  
  @JsonProperty("nodes")
  public ArrayList<Node> nodes;
  
  @JsonProperty("meshes")
  public ArrayList<Mesh> meshes;
  
  @JsonProperty("cameras")
  public ArrayList<Camera> cameras;
  
  @JsonProperty("materials")
  public ArrayList<Material> materials;
  
  @JsonProperty("skybox")
  public Skybox skybox;
  
  @JsonProperty("environment")
  public ArrayList<Environment> environments;
  
  @JsonProperty("shaders")
  public ArrayList<Shader> shaders;

  // TODO: Animations

  public void resolveIds() {
    defaultScene = scenes.get(defaultSceneId);
    
    for(Scene s: scenes) {
      s.resolveIds(this);
    }
    
    for(Node n: nodes) {
      n.resolveIds(this);
    }

    for(Material m: materials) {
      m.resolveIds(this);
    }
    
    for(Mesh m: meshes) {
      m.resolveIds(this);
    }
  }
  
  public void updateIds() {
    defaultSceneId = scenes.indexOf(defaultScene);
    
    for(Scene s: scenes) {
      s.updateIds(this);
    }

    for(Node n: nodes) {
      n.updateIds(this);
    }

    for(Material m: materials) {
      m.updateIds(this);
    }
    
    for(Mesh m: meshes) {
      m.updateIds(this);
    }
}
}
