package com.samsung.dali.modelconverter.view.parts;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelconverter.view.GlobalParts;

public class DliTextPart {
  private Text text;

  public DliTextPart() {
    GlobalParts.Get().setDliTextPart(this);
  }

  @PostConstruct
  public void createComposite(Composite parent) {
    parent.setLayout(new FormLayout());

    text = new Text(parent, SWT.BORDER | SWT.MULTI);
    FormData fd_text = new FormData();
    fd_text.bottom = new FormAttachment(100, -10);
    fd_text.right = new FormAttachment(100, -5);
    fd_text.top = new FormAttachment(0, 5);
    fd_text.left = new FormAttachment(0, 5);
    text.setLayoutData(fd_text);

  }
}
