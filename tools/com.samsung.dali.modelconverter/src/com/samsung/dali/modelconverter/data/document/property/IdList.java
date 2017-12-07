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

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import com.samsung.dali.modelconverter.data.document.property.Property.ValueProvider;

public class IdList<T> implements ValueProvider {

  public IdList(List<T> values, boolean nullable) {
    mValues = new ArrayList<String>();
    if (nullable) {
      mValues.add("(none)");
    }
    for (Object o : values) {
      mValues.add(o.toString());
    }
  }

  public IdList(List<T> values) {
    this(values, false);
  }

  @Override
  public Collection<?> getValues() {
    return mValues;
  }

  List<String> mValues;
}
