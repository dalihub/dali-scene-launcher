package com.samsung.dali.modelconverter.parts;

import java.net.URL;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.TreeViewerColumn;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.resource.LocalResourceManager;
import org.eclipse.jface.resource.ResourceManager;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.swt.graphics.Image;


public class ResourcesTreeNode {
	

	private static class ViewerLabelProvider extends LabelProvider {
        private ImageDescriptor directoryImage;
        private ResourceManager resourceManager;
        public ViewerLabelProvider( ImageDescriptor directoryImage ) {
        	this.directoryImage = directoryImage;
        }
        protected ResourceManager getResourceManager() {
            if(resourceManager == null) {
                resourceManager = new LocalResourceManager(JFaceResources.getResources());
            }
            return resourceManager;
        }
        
		public Image getImage(Object element) {
			return getResourceManager().createImage(directoryImage);
			//return super.getImage(element);
		}
		public String getText(Object element) {
			return super.getText(element);
		}
	}
	
	private static class TreeContentProvider implements ITreeContentProvider {
		
		public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
		}
		
		public void dispose() {
		}
		
		public Object[] getElements(Object inputElement) {
			
			return getChildren(inputElement);
		}
		
		public Object[] getChildren(Object parentElement) {
			return new Object[] { "item_0", "item_1", "item_2" };
		}
		
		public Object getParent(Object element) {
			return null;
		}
		
		public boolean hasChildren(Object element) {
			return getChildren(element).length > 0;
		}
	}
	
	
	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new FillLayout(SWT.HORIZONTAL));
		Bundle bundle = FrameworkUtil.getBundle(ViewerLabelProvider.class);
		ImageDescriptor imgdesc = ImageDescriptor.createFromURL(FileLocator.find(bundle, new Path("icons/save_edit.png"), null));
		
		/*TreeViewer treeViewer = new TreeViewer(parent, SWT.BORDER);
		Tree tree = treeViewer.getTree();
		treeViewer.setLabelProvider(new ViewerLabelProvider());
		treeViewer.setContentProvider(new TreeContentProvider());*/
		
        TreeViewer treeViewer = new TreeViewer(parent);
        treeViewer.setContentProvider(new TreeContentProvider());
        treeViewer.getTree().setHeaderVisible(true);
        treeViewer.getTree().setLinesVisible(true);
        treeViewer.setLabelProvider(new ViewerLabelProvider(imgdesc));

        /*TreeViewerColumn viewerColumn = new TreeViewerColumn(treeViewer, SWT.NONE);
        viewerColumn.getColumn().setWidth(300);
        viewerColumn.getColumn().setText("Names");
        viewerColumn.setLabelProvider(new ColumnLabelProvider());*/

        treeViewer.setInput(new String[] { "Simon Scholz", "Lars Vogel", "Dirk Fauth", "Wim Jongman", "Tom Schindl" });

        //GridLayoutFactory.fillDefaults().generateLayout(parent);
		

	}
}
