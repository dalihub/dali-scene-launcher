package com.samsung.dali.modelconverter.controller;

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

import java.lang.reflect.Field;

import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.e4.ui.workbench.modeling.EPartService.PartState;

public class PartsHelper {

  @SuppressWarnings("unchecked")
  public static <T> T getPart(EPartService parts, Class<T> clazz) {
    try {
      Field idField = clazz.getField("sId");
      String id = (String) idField.get(null);
      MPart part = parts.showPart(id, PartState.VISIBLE);
      return (T) part.getObject();
    } catch (IllegalArgumentException | IllegalAccessException | NoSuchFieldException | SecurityException e) {

      throw new RuntimeException(e.getMessage());
    }
  }
}