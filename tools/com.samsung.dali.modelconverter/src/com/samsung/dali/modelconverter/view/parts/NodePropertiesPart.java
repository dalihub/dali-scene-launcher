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

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;

public class NodePropertiesPart {

  @PostConstruct
  public void createComposite(Composite parent) {
    parent.setLayout(new GridLayout(1, false));

    mParent = parent;
    resetProperties();

    sActiveInstance = this;
  }

  public void resetProperties() {

    if (mProperties != null && !mProperties.isDisposed()) {
      mProperties.dispose();
    }

    mProperties = new Composite(mParent, SWT.FILL);
    mProperties.setLayoutData(new GridData(SWT.LEFT, SWT.TOP, true, true, 1, 1));
    mProperties.setLayout(new GridLayout(2, false));
  }

  public Composite getComposite() {
    return mProperties;
  }

  static NodePropertiesPart sActiveInstance;

  private Composite mParent;
  private Composite mProperties;
}
