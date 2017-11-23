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
