package com.samsung.dali.modelconverter.data;

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

import java.io.File;

public class GlobalData {

  static public void initialize() {
    if( sData == null) {
      sData = new GlobalData();
    }

    if(System.getProperty("os.name").toLowerCase().contains("windows"))
    {
      sData.mIsWindows = true;
    }
    sData.getRootTizenPath();
    sData.mTizenPath = sData.mRootTizenPath + "tools" + File.separator + "ide" + File.separator + "bin" + File.separator;
    sData.mSdbPath = sData.mRootTizenPath + "tools" + File.separator;
  }

  static public GlobalData get() {
    if( sData == null) {
      sData = new GlobalData();
    }
    return sData;
  }

  public boolean isWindows() {
    return mIsWindows;
  }

  public void setIsWindows(boolean isWindows) {
    mIsWindows = isWindows;
  }

  public String setSdbPath() {
    return mSdbPath;
  }

  public void setSdbPath(String sdbPath) {
    mSdbPath = sdbPath;
  }

  public String setInstallPackagePath() {
    return mInstallPackagePath;
  }

  public void setInstallPackagePath(String installPackagePath) {
    mInstallPackagePath = installPackagePath;
  }

  public String setTizenPath() {
    return mTizenPath;
  }

  public void setTizenPath(String tizenPath) {
    mTizenPath = tizenPath;
  }

  /*
   * Registry values
   * */
  public String getRootTizenPath() {
    sData.mRootTizenPath = Settings.Get().getString( Settings.TIZEN_CLI_ROOT_PATH );
    return mRootTizenPath;
  }

  public void setRootTizenPath(String mRootTizenPath) {
    Settings.Get().setString(Settings.TIZEN_CLI_ROOT_PATH, mRootTizenPath);
    this.mRootTizenPath = mRootTizenPath;
  }

  static GlobalData sData;

  boolean mIsWindows = false;
  String mRootTizenPath = "";
  String mTizenPath = "";
  String mSdbPath = "";
  String mInstallPackagePath = "";
}
