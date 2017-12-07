package com.samsung.dali.modelconverter.view.parts;

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

  public static NodePropertiesPart getNodePropertiesPart() {
    if (NodePropertiesPart.sActiveInstance == null) {
      createPart("com.samsung.dali.modelconverter.part.nodeproperties");
      assert NodePropertiesPart.sActiveInstance != null;
    }
    return NodePropertiesPart.sActiveInstance;
  }
  
  static void createPart(String id)
  {
    Bundle bundle = FrameworkUtil.getBundle(EPartService.class);
    BundleContext bundleContext = bundle.getBundleContext();
    IEclipseContext eclipseContext = EclipseContextFactory.getServiceContext(bundleContext);
    EPartService partService = (EPartService)eclipseContext.get(EPartService.class);
    partService.showPart(id, PartState.CREATE);
  }
}