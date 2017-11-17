package com.samsung.dali.modelConverter.parts;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Composite;

public class OutputPart {

  //  Bundle bundle = FrameworkUtil.getBundle(EPartService.class);
  //  BundleContext bundleContext = bundle.getBundleContext();
  //  IEclipseContext eclipseCtx = EclipseContextFactory.getServiceContext(bundleContext);
  //  EPartService service = (EPartService)eclipseCtx.get(EPartService.class.getName());
  //  final MainPart mainPart = (MainPart) service.findPart("com.samsung.dali.modelConverter.part.Main").getObject();
  public static OutputPart sActiveInstance;

  @PostConstruct
  public void createControls(Composite parent) {

    mText = new StyledText(parent, SWT.BORDER | SWT.MULTI | SWT.V_SCROLL);
    mText.setAlwaysShowScrollBars(false);
    mText.setWordWrap(true);

    // do layout
    FormLayout layout = new FormLayout();
    parent.setLayout(layout);

    FormData textData = new FormData();
    textData.left = new FormAttachment(0, 0);
    textData.right = new FormAttachment(100, 0);
    textData.top = new FormAttachment(0, 0);
    textData.bottom = new FormAttachment(100, 0);
    mText.setLayoutData(textData);
    
    sActiveInstance = this;
  }

  public StyledText getOutputPanel()
  {
    return mText;    
  }
  
  // private
  private StyledText mText;

}
