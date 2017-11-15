package com.samsung.dali.modelConverter.data;

import java.io.File;

import org.eclipse.swt.widgets.Shell;

public class GlobalData {
	static public GlobalData data;
	public boolean isWindows = false;
	public String CurrentProfile = "";
	public String TizenPath = "";
	public String SdbPath = "";
	public String InstallPackagePath = "";
	
	static public void Initialize(Shell shell) {
		if( data == null) {
			data = new GlobalData();
		}
		if(System.getProperty("os.name").toLowerCase().contains("windows")) 
		{
			data.isWindows = true;
		}
		SettingsClass.CheckTizenCliPath( shell, false);
		data.TizenPath = SettingsClass.Get().GetString(SettingsClass.TIZEN_CLI_PATH)+ "tools" + File.separator + "ide" + File.separator + "bin" + File.separator;
		data.SdbPath = SettingsClass.Get().GetString(SettingsClass.TIZEN_CLI_PATH)+ "tools" + File.separator;
		data.CurrentProfile = SettingsClass.Get().GetString(SettingsClass.TIZEN_CURRENT_PROFILE);
	}
	
	static public GlobalData Get() {
		if( data == null) {
			data = new GlobalData();
		}
		return data;
	}
}
