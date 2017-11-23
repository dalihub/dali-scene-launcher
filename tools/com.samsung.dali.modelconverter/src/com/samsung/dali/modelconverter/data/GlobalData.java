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

    sData.setRootTizenPath(sData.getRootTizenPath());
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

  public String getSdbPath() {
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

  public String getTizenPath() {
    return mTizenPath;
  }

  public void setTizenPath(String tizenPath) {
    mTizenPath = tizenPath;
  }

  /*
   * Registry values
   * */
  public String getRootTizenPath() {
    return Settings.Get().getString( Settings.TIZEN_CLI_ROOT_PATH );
  }

  public void setRootTizenPath(String rootTizenPath) {
    Settings.Get().setString(Settings.TIZEN_CLI_ROOT_PATH, rootTizenPath);

    mTizenPath = rootTizenPath + File.separator + "tools" + File.separator + "ide" + File.separator + "bin" + File.separator + "tizen";
    mSdbPath = rootTizenPath + File.separator + "tools" + File.separator + "sdb";

    if(sData.isWindows())
    {
      mTizenPath += ".bat";
      mSdbPath += ".exe";
    }
  }

  private static GlobalData sData;

  private boolean mIsWindows = false;
  private String mTizenPath = "";
  private String mSdbPath = "";
  private String mInstallPackagePath = "";
}
