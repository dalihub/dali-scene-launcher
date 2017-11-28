package com.samsung.dali.modelconverter.controller;

import java.util.ArrayList;

import org.eclipse.jface.viewers.ITreeContentProvider;

import com.samsung.dali.modelconverter.data.document.Document;
import com.samsung.dali.modelconverter.data.document.Node;

public class SceneGraphContentProvider implements ITreeContentProvider {

	public SceneGraphContentProvider(Document doc) {
		document = doc;
	}

	public Object getDocument() {
		return document;
	}

	@Override
	public Object[] getElements(Object inputElement) {
		return getChildren(inputElement);
	}

	@Override
	public Object[] getChildren(Object parentElement) {
		ArrayList<Object> kids = new ArrayList<Object>();
		if (parentElement == document) {
			kids.addAll(document.getRootNodes());
			kids.addAll(document.getCameras());

			if (kids.isEmpty()) {
				kids.add("<The document is empty>");
			}
		}
		else if(parentElement instanceof Node) {
		  kids.addAll(document.getNodeChildren((Node)parentElement));
		}
		return kids.toArray();
	}

	@Override
	public Object getParent(Object element) {

		if (element instanceof Node) {
			return ((Node) element).getParent();
		}
		return null;
	}

	@Override
	public boolean hasChildren(Object element) {

		return getChildren(element).length > 0;
	}

	private Document document;
}
