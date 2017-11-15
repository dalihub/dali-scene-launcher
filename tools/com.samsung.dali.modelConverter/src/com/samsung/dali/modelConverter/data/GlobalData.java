package com.samsung.dali.modelConverter.data;

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

import org.eclipse.swt.widgets.Shell;

import com.samsung.dali.modelConverter.parts.ConsolePart;
import com.samsung.dali.modelConverter.parts.NodeTreePart;

public class GlobalData {
  static GlobalData sData;

  boolean mIsWindows = false;
  String mCurrentProfile = "";
  String mTizenPath = "";
  String mSdbPath = "";
  String mInstallPackagePath = "";

  ConsolePart mConsolePart;
  NodeTreePart mNodeTreePart;


  static public void Initialize(Shell shell) {
    if( sData == null) {
      sData = new GlobalData();
    }
    if(System.getProperty("os.name").toLowerCase().contains("windows"))
    {
      sData.mIsWindows = true;
    }
    SettingsClass.CheckTizenCliPath( shell, false);
    sData.mTizenPath = SettingsClass.Get().GetString( SettingsClass.TIZEN_CLI_PATH )+ "tools" + File.separator + "ide" + File.separator + "bin" + File.separator;
    sData.mSdbPath = SettingsClass.Get().GetString( SettingsClass.TIZEN_CLI_PATH )+ "tools" + File.separator;
    sData.mCurrentProfile = SettingsClass.Get().GetString( SettingsClass.TIZEN_CURRENT_PROFILE );
    sData.mInstallPackagePath = SettingsClass.Get().GetString( SettingsClass.TIZEN_CURRENT_WORKPATH );
  }

  static public GlobalData Get() {
    if( sData == null) {
      sData = new GlobalData();
    }
    return sData;
  }

  public boolean IsWindows() {
    return mIsWindows;
  }

  public String GetCurrentProfile() {
    return mCurrentProfile;
  }

  public String GetTizenPath() {
    return mTizenPath;
  }

  public String GetSdbPath() {
    return mSdbPath;
  }

  public String GetInstallPackagePath() {
    return mInstallPackagePath;
  }

  public ConsolePart getConsolePart() {
    return mConsolePart;
  }

  public NodeTreePart getNodeTreePart() {
    return mNodeTreePart;
  }
  
  public void SetIsWindows(boolean isWindows) {
    mIsWindows = isWindows;
  }

  public void SetCurrentProfile(String currentProfile) {
    mCurrentProfile = currentProfile;
  }

  public void SetTizenPath(String tizenPath) {
    mTizenPath = tizenPath;
  }

  public void SetSdbPath(String sdbPath) {
    mSdbPath = sdbPath;
  }

  public void SetInstallPackagePath(String installPackagePath) {
    mInstallPackagePath = installPackagePath;
  }

  public void setConsolePart(ConsolePart consolePart) {
    mConsolePart = consolePart;
  }

  public void setNodeTreePart(NodeTreePart nodeTreePart) {
    mNodeTreePart = nodeTreePart;
  }
}
