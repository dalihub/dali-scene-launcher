package com.samsung.dali.modelconverter.data.document.property;

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

import java.util.Map;
import com.samsung.dali.modelconverter.data.document.property.Property.IValueProcessor;
import com.samsung.dali.modelconverter.data.document.ValueHolder;

public class MapElementSetter implements IValueProcessor {

  public MapElementSetter( String key ) {
    mKey = key;
  }

  @Override
  public Object present(Object in) {
    Map<String,ValueHolder> map = (Map<String,ValueHolder>)in;
    ValueHolder value = map.get( mKey );
    if( null != value )
    {
      return (Object)value.get();
    }
    return null;
  }

  @Override
  public Object store(Object in) {
    Map<String,ValueHolder> map = (Map<String,ValueHolder>)in;
    ValueHolder value = map.get( mKey );
    if( null != value )
    {
      return (Object)value.get();
    }
    return null;
  }

  private String mKey;

}
