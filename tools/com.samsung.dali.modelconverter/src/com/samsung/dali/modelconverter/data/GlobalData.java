package com.samsung.dali.modelconverter.data;

import java.io.File;

public class GlobalData {
  static GlobalData sData;

  boolean mIsWindows = false;
  String mRootTizenPath = "";
  String mTizenPath = "";
  String mSdbPath = "";
  String mInstallPackagePath = "";


  static public void Initialize() {
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

  static public GlobalData Get() {
    if( sData == null) {
      sData = new GlobalData();
    }
    return sData;
  }

  public boolean IsWindows() {
    return mIsWindows;
  }
  
  public void SetIsWindows(boolean isWindows) {
    mIsWindows = isWindows;
  }
  
  public String GetSdbPath() {
    return mSdbPath;
  }

  public void SetSdbPath(String sdbPath) {
    mSdbPath = sdbPath;
  }
  
  public String GetInstallPackagePath() {
    return mInstallPackagePath;
  }
  
  public void SetInstallPackagePath(String installPackagePath) {
    mInstallPackagePath = installPackagePath;
  }
  
  public String GetTizenPath() {
    return mTizenPath;
  }

  public void SetTizenPath(String tizenPath) {
    mTizenPath = tizenPath;
  }

  /*
   * Registry values
   * */
  public String getRootTizenPath() {
    sData.mRootTizenPath = SettingsClass.Get().GetString( SettingsClass.TIZEN_CLI_ROOT_PATH );
    return mRootTizenPath;
  }

  public void setRootTizenPath(String mRootTizenPath) {
    SettingsClass.Get().SetString(SettingsClass.TIZEN_CLI_ROOT_PATH, mRootTizenPath);
    this.mRootTizenPath = mRootTizenPath;
  }

}
