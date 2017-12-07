package com.samsung.dali.modelconverter.view.parts;

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

import org.eclipse.e4.core.contexts.EclipseContextFactory;
import org.eclipse.e4.core.contexts.IEclipseContext;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.e4.ui.workbench.modeling.EPartService.PartState;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.FrameworkUtil;

public class GlobalParts {

  public static DliTextPart getDliTextPart() {
    if (DliTextPart.sActiveInstance == null) {
      createPart("com.samsung.dali.modelconverter.part.dlitext");
      assert DliTextPart.sActiveInstance != null;
    }
    return DliTextPart.sActiveInstance;
  }

  public static SceneGraphPart getSceneGraphPart() {
    if (SceneGraphPart.sActiveInstance == null) {
      createPart("com.samsung.dali.modelconverter.part.scenegraph");
      assert SceneGraphPart.sActiveInstance != null;
    }
    return SceneGraphPart.sActiveInstance;
  }

  public static PropertiesPart getPropertiesPart() {
    if (PropertiesPart.sActiveInstance == null) {
      createPart("com.samsung.dali.modelconverter.part.properties");
      assert PropertiesPart.sActiveInstance != null;
    }
    return PropertiesPart.sActiveInstance;
  }

  static void createPart(String id) {
    Bundle bundle = FrameworkUtil.getBundle(EPartService.class);
    BundleContext bundleContext = bundle.getBundleContext();
    IEclipseContext eclipseContext = EclipseContextFactory.getServiceContext(bundleContext);
    EPartService partService = (EPartService) eclipseContext.get(EPartService.class);
    partService.showPart(id, PartState.CREATE);
  }
}