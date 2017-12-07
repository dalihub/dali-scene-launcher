package com.samsung.dali.modelconverter.view.parts;

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
