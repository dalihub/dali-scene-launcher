package com.samsung.dali.modelconverter.controller;

import org.eclipse.e4.core.services.events.IEventBroker;
import org.eclipse.e4.ui.workbench.lifecycle.PostContextCreate;
import org.eclipse.equinox.app.IApplicationContext;

import com.samsung.dali.modelconverter.data.GlobalData;

public class StartUp {
  @PostContextCreate
  void postContextCreate(final IEventBroker eventBroker, IApplicationContext context) {
    GlobalData.initialize();
    TizenPathDialogController.verifyTizenPath(null, false);
  }
}
