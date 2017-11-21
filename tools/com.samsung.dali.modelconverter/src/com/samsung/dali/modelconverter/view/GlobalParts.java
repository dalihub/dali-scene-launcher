package com.samsung.dali.modelconverter.view;

import com.samsung.dali.modelconverter.view.parts.DliTextPart;
import com.samsung.dali.modelconverter.view.parts.SceneGraphPart;

public class GlobalParts {

  static GlobalParts sData;

  DliTextPart mDliTextPart;
  SceneGraphPart mSceneGraphPart;

  static public GlobalParts Get() {
    if( sData == null) {
      sData = new GlobalParts();
    }
    return sData;
  }

  public DliTextPart getDliTextPart() {
    return mDliTextPart;
  }

  public SceneGraphPart getSceneGraphPart() {
    return mSceneGraphPart;
  }

  public void setDliTextPart(DliTextPart mDliTextPart) {
    this.mDliTextPart = mDliTextPart;
  }

  public void setSceneGraphPart(SceneGraphPart mSceneGraphPart) {
    this.mSceneGraphPart = mSceneGraphPart;
  }

}