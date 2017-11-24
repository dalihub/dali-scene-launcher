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

import org.eclipse.core.runtime.preferences.ConfigurationScope;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

public class Settings {

  private Settings() {
    preferences = ConfigurationScope.INSTANCE.getNode("com.samsung.dali.modelconverter");
  }

  static public Settings Get() {
    if( settings == null) {
      settings = new Settings();
    }
    return settings;
  }

  public String getString( String ekey ) {
    Preferences pref = preferences.node( "settings" );
    return pref.get(ekey, "");
  }

  public void setString( String ekey, String evalue ) {
    Preferences pref = preferences.node( "settings" );
    pref.put( ekey, evalue);
    try {
      preferences.flush();
    } catch (BackingStoreException e) {
      e.printStackTrace();
    }
  }

  public int getInteger( String ekey ) {
    Preferences pref = preferences.node( "settings" );
    return pref.getInt( ekey, 0 );
  }

  public void setInteger( String ekey, int evalue ) {
    Preferences pref = preferences.node( "settings" );
    pref.putInt( ekey, evalue );
    try {
      preferences.flush();
    } catch (BackingStoreException e) {
      e.printStackTrace();
    }
  }

  public static final String TIZEN_CLI_ROOT_PATH = "TIZEN_CLI_ROOT_PATH";
  public static final String TIZEN_CURRENT_PROFILE = "TIZEN_CURRENT_PROFILE";
  public static final String TIZEN_CURRENT_WORKPATH = "TIZEN_CURRENT_WORKPATH";

  static private Settings settings;
  private Preferences preferences;
}
