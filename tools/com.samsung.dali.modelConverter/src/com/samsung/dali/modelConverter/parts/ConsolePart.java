package com.samsung.dali.modelConverter.parts;

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
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelConverter.data.GlobalData;

import org.eclipse.swt.layout.FillLayout;

public class ConsolePart {
  private Text mConsoleTxt;

  @PostConstruct
  public void createComposite(Composite parent) {
    GlobalData.Get().setConsolePart(this);
    parent.setLayout(new FillLayout(SWT.HORIZONTAL));

    mConsoleTxt = new Text(parent, SWT.BORDER | SWT.V_SCROLL | SWT.MULTI);

  }
  
  public void AppendText( String str )
  {
    mConsoleTxt.append(str);
  }

}
