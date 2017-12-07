package com.samsung.dali.modelconverter.controller;

import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.widgets.TreeItem;

import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.property.Property;

public class SceneGraphSelectionChangedListener implements ISelectionChangedListener {

  SceneGraphSelectionChangedListener(Document doc, Property.Receiver receiver) {
    mDocument = doc;
    mReceiver = receiver;
  }

  @Override
  public void selectionChanged(SelectionChangedEvent event) {

    mReceiver.resetProperties();
    TreeViewer tv = (TreeViewer) event.getSource();
    TreeItem[] selection = tv.getTree().getSelection();
    if (selection.length == 1) {
      TreeItem ti = selection[0];
      ((Property.Provider) ti.getData()).provideProperties(mDocument, mReceiver);
    }
  }

  private Document mDocument;
  private Property.Receiver mReceiver;
}
