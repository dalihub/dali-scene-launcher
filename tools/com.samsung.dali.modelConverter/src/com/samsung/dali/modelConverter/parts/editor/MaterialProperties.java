package com.samsung.dali.modelConverter.parts.editor;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

public class MaterialProperties {
  private Text mConsoleTxt;
  @PostConstruct
  public void createComposite(Composite parent) {
    parent.setLayout(new FillLayout(SWT.HORIZONTAL));

    mConsoleTxt = new Text(parent, SWT.BORDER | SWT.V_SCROLL | SWT.MULTI);

  }
}
