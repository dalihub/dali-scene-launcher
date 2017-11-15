package com.samsung.dali.modelConverter.parts;

import javax.annotation.PostConstruct;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.layout.FillLayout;

public class ConsolePart {
  private Text mConsoleTxt;


  @PostConstruct
  public void createComposite(Composite parent) {
    parent.setLayout(new FillLayout(SWT.HORIZONTAL));

    mConsoleTxt = new Text(parent, SWT.BORDER | SWT.V_SCROLL | SWT.MULTI);

  }

}
