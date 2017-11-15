package com.samsung.dali.modelconverter.data;

import org.eclipse.core.runtime.preferences.ConfigurationScope;
import org.eclipse.jface.dialogs.MessageDialog;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

public class SettingsClass {
	
	static private SettingsClass settings;
	private Preferences preferences;
	
	private SettingsClass() {
		preferences = ConfigurationScope.INSTANCE.getNode("com.samsung.dali.modelConverter");
	}
	
	static public SettingsClass Get() {
		if( settings == null) {
			settings = new SettingsClass();
		}
		return settings;
	}
	
	public String GetString( String ekey ) {
		Preferences pref = preferences.node( "settings" );
		return pref.get(ekey, "");
	}
	
	public void SetString( String ekey, String evalue ) {
		Preferences pref = preferences.node( "settings" );
		pref.put( ekey, evalue);
		try {
			preferences.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}
	}
	
	public int GetInteger( String ekey ) {
		Preferences pref = preferences.node( "settings" );
		return pref.getInt( ekey, 0 );
	}
	
	public void SetInteger( String ekey, int evalue ) {
		Preferences pref = preferences.node( "settings" );
		pref.putInt( ekey, evalue );
		try {
			preferences.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}
	}
}
