package com.samsung.dali.modelConverter.data;

import java.io.File;

import org.eclipse.swt.widgets.Shell;

public class GlobalData {
  static private GlobalData sData;
  public boolean mIsWindows = false;
  public String mCurrentProfile = "";
  public String mTizenPath = "";
  public String mSdbPath = "";
  public String mInstallPackagePath = "";

  static public void Initialize(Shell shell) {
    if( sData == null) {
      sData = new GlobalData();
    }
    if(System.getProperty("os.name").toLowerCase().contains("windows")) 
    {
      sData.mIsWindows = true;
    }
    SettingsClass.CheckTizenCliPath( shell, false);
    sData.mTizenPath = SettingsClass.Get().GetString(SettingsClass.TIZEN_CLI_PATH)+ "tools" + File.separator + "ide" + File.separator + "bin" + File.separator;
    sData.mSdbPath = SettingsClass.Get().GetString(SettingsClass.TIZEN_CLI_PATH)+ "tools" + File.separator;
    sData.mCurrentProfile = SettingsClass.Get().GetString(SettingsClass.TIZEN_CURRENT_PROFILE);
  }

  static public GlobalData Get() {
    if( sData == null) {
      sData = new GlobalData();
    }
    return sData;
  }
}
