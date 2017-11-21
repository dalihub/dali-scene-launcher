package com.samsung.dali.modelconverter.view.parts;

import java.util.Arrays;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.inject.Inject;

import org.eclipse.e4.ui.di.Focus;
import org.eclipse.e4.ui.di.Persist;
import org.eclipse.e4.ui.model.application.ui.MDirtyable;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormAttachment;

public class NodeTreePart {
	private Text text;



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