package com.samsung.dali.modelConverter.parts;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;

public class AnimationTreePart {
  @PostConstruct
  public void createComposite( Composite parent ) {
    parent.setLayout(new FillLayout(SWT.HORIZONTAL));

    Tree tree = new Tree(parent, SWT.BORDER);

    TreeItem itemN1 = new TreeItem(tree, SWT.NONE);
    itemN1.setText("Node1");

    TreeItem sItem1 = new TreeItem(itemN1, SWT.NONE);
    sItem1.setText("subNode1");
    itemN1.setExpanded(true);

    TreeItem itemN2 = new TreeItem(tree, SWT.NONE);
    itemN2.setText("Node2");
  }
}
