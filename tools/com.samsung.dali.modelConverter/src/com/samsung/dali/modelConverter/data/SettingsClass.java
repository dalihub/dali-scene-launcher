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

import org.eclipse.core.runtime.preferences.ConfigurationScope;
import org.eclipse.swt.widgets.Shell;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import com.samsung.dali.modelConverter.windows.TizenPathDialog;

public class SettingsClass {
  public static final String TIZEN_CLI_PATH = "TIZEN_CLI_PATH";
  public static final String TIZEN_CURRENT_PROFILE = "TIZEN_CURRENT_PROFILE";
  public static final String TIZEN_CURRENT_WORKPATH = "TIZEN_CURRENT_WORKPATH";

  static private SettingsClass settings;
  private Preferences preferences;

  private SettingsClass() {
    preferences = ConfigurationScope.INSTANCE.getNode("com.samsung.dali.modelConverter");
  }
  static public void CheckTizenCliPath( Shell shell, boolean eforce) {
    String str  = Get().GetString( TIZEN_CLI_PATH );
    if(str.isEmpty() || eforce)
    {
      TizenPathDialog pathTizen = new TizenPathDialog(shell);
      pathTizen.open();
    }
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
