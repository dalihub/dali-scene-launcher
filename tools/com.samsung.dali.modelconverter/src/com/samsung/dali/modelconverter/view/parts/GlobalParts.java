package com.samsung.dali.modelconverter.view.parts;

public class GlobalParts {

  public static DliTextPart getDliTextPart() {
    return DliTextPart.sActiveInstance;
  }

  public static SceneGraphPart getSceneGraphPart() {
    return SceneGraphPart.sActiveInstance;
  }

}