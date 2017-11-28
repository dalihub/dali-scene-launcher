package com.samsung.dali.modelconverter.data.document;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

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

  public Map<String, Node> collectNodes() {
    Map<String, Node> map = new TreeMap<String, Node>();
    for (Scene s : getScenes()) {
      Integer rootId = s.getRootId();
      if (rootId != null) {
        Node n = getNodes().get(rootId.intValue());
        n.collect(this, "", map);
      }
    }
    return map;
  }

  public List<Node> getRootNodes() {
    ArrayList<Node> roots = new ArrayList<Node>();
    for (Scene s : getScenes()) {
      Integer rootId = s.getRootId();
      if (rootId != null) {
        roots.add(getNodes().get(rootId.intValue()));
      }
    }
    return roots;
  }

  public List<Node> getNodeChildren(Node n) {
    ArrayList<Node> kids = new ArrayList<Node>();
    for (Integer i : n.getChildIds()) {
      kids.add(getNodes().get(i.intValue()));
    }
    return kids;
  }

  @JsonIgnoreProperties(ignoreUnknown = true)

  @JsonProperty("asset")
  private Asset mAsset = new Asset();

  @JsonProperty("scene")
  private int mDefaultSceneId = 0;

  @JsonProperty("scenes")
  private ArrayList<Scene> mScenes = new ArrayList<Scene>();

  @JsonProperty("nodes")
  private ArrayList<Node> mNodes = new ArrayList<Node>();

  @JsonProperty("cameras")
  private ArrayList<Camera> mCameras = new ArrayList<Camera>();

  @JsonProperty("skybox")
  private Skybox mSkybox;

  @JsonProperty("meshes")
  private ArrayList<Mesh> mMeshes = new ArrayList<Mesh>();

  @JsonProperty("shaders")
  private ArrayList<Shader> mShaders = new ArrayList<Shader>();

  @JsonProperty("materials")
  private ArrayList<Material> mMaterials = new ArrayList<Material>();

  @JsonProperty("environment")
  private ArrayList<Environment> mEnvironments = new ArrayList<Environment>();

  // TODO: Animations
}
