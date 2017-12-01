package com.samsung.dali.modelconverter.data.document;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/*
 * @brief A dli document, serialisable to / from JSON (glTF).
 */
public class Document {

  static public Document fromDli(String dli) throws JsonParseException, JsonMappingException, IOException {
    ObjectMapper mapper = new ObjectMapper();
    mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);

    Document d = mapper.readValue(dli, Document.class);
    d.setNodeParents();
    d.setIds();
    return d;
  }

  public String toDliString() throws JsonProcessingException {
    ObjectMapper mapper = new ObjectMapper();
    return mapper.writerWithDefaultPrettyPrinter().writeValueAsString(this);
  }

  public Asset getAsset() {
    return mAsset;
  }

  public void setAsset(Asset asset) {
    mAsset = asset;
  }

  @JsonProperty("scene")
  public int getDefaultSceneId() {
    return mDefaultSceneId;
  }

  public void setDefaultSceneId(int defaultSceneId) {
    mDefaultSceneId = defaultSceneId;
  }

  public ArrayList<Scene> getScenes() {
    return mScenes;
  }

  public void setScenes(ArrayList<Scene> scenes) {
    mScenes = scenes;
  }

  public ArrayList<Node> getNodes() {
    return mNodes;
  }

  public Node getNode(int i) {
    return mNodes.get(i);
  }

  public void setNodes(ArrayList<Node> nodes) {
    mNodes = nodes;
  }

  public ArrayList<Camera> getCameras() {
    return mCameras;
  }

  public void setCameras(ArrayList<Camera> cameras) {
    mCameras = cameras;
  }

  public Skybox getSkybox() {
    return mSkybox;
  }

  public void setSkybox(Skybox skybox) {
    mSkybox = skybox;
  }

  public ArrayList<Mesh> getMeshes() {
    return mMeshes;
  }

  public void setMeshes(ArrayList<Mesh> meshes) {
    mMeshes = meshes;
  }

  public ArrayList<Material> getMaterials() {
    return mMaterials;
  }

  public void setMaterials(ArrayList<Material> materials) {
    mMaterials = materials;
  }

  public ArrayList<Shader> getShaders() {
    return mShaders;
  }

  public void setShaders(ArrayList<Shader> shaders) {
    mShaders = shaders;
  }

  @JsonProperty("environment")
  public ArrayList<Environment> getEnvironments() {
    return mEnvironments;
  }

  public void setEnvironments(ArrayList<Environment> environments) {
    mEnvironments = environments;
  }

  public void setNodeParents() {
    for (Node n : mNodes) {
      for (Integer i : n.getChildIds()) {
        mNodes.get(i.intValue()).setParent(n);
      }
    }
  }

  public void setIds() {
    int id = 1;
    for (Scene s : mScenes) {
      s.setId(id);
      ++id;
    }

    id = 1;
    for (Camera c : mCameras) {
      c.setId(id);
      ++id;
    }
  }

  public void organizeOrphans() {
    for (int i = 0; i < mNodes.size(); ++i) {
      Node n = mNodes.get(i);
      if (n.getParent() == null) {
        boolean found = false;
        for (Scene s : mScenes) {
          if (s.getRootId() == i) {
            found = true;
            break;
          }
        }

        if (!found) {
          Scene s = new Scene();
          s.setId(mScenes.size() + 1);
          s.setRootId(i);
          s.setOrphan(true);
          mScenes.add(s);
        }
      }
    }
  }

  public List<Node> getNodeChildren(Node n) {
    ArrayList<Node> kids = new ArrayList<Node>();
    for (Integer i : n.getChildIds()) {
      kids.add(getNodes().get(i.intValue()));
    }
    return kids;
  }

  @JsonIgnoreProperties(ignoreUnknown = true)

  private Asset mAsset = new Asset();

  private int mDefaultSceneId = 0;

  private ArrayList<Scene> mScenes = new ArrayList<Scene>();
  private ArrayList<Node> mNodes = new ArrayList<Node>();
  private ArrayList<Camera> mCameras = new ArrayList<Camera>();

  private Skybox mSkybox;

  private ArrayList<Mesh> mMeshes = new ArrayList<Mesh>();
  private ArrayList<Shader> mShaders = new ArrayList<Shader>();
  private ArrayList<Material> mMaterials = new ArrayList<Material>();

  private ArrayList<Environment> mEnvironments = new ArrayList<Environment>();

  // TODO: Animations
}
