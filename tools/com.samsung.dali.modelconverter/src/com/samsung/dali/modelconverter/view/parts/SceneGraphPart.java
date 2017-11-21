package com.samsung.dali.modelconverter.view.parts;

import javax.annotation.PostConstruct;

import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;

public class SceneGraphPart {



	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new FormLayout());
		
		TreeViewer treeViewer = new TreeViewer(parent, SWT.BORDER);
		Tree tree = treeViewer.getTree();
		FormData fd_tree = new FormData();
		fd_tree.bottom = new FormAttachment(100);
		fd_tree.right = new FormAttachment(100);
		fd_tree.top = new FormAttachment(0);
		fd_tree.left = new FormAttachment(0);
		tree.setLayoutData(fd_tree);

	}


	
}