package com.samsung.dali.modelConverter;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import javax.annotation.PostConstruct;

import org.eclipse.e4.ui.di.Focus;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Text;

import com.samsung.dali.modelConverter.model.ModelConverterLogic;
import com.samsung.dali.modelConverter.view.PathSelectionWidget;

public class MainPart {

	PathSelectionWidget pswImport;
	PathSelectionWidget pswExport;
	Button btnDoExport;

	StyledText textGlTF;
	
	@PostConstruct
    public void createControls(Composite parent) {
		
		// Create controls
		pswImport = new PathSelectionWidget(parent, 0, "Select file to import");
		pswImport.setFilterExtensions(new String[] { "*.dae" });
		
		pswExport = new PathSelectionWidget(parent, 0, "Select file to export to");
		pswExport.setFileDialogStyle(SWT.SAVE);
		
		btnDoExport = new Button(parent, 0);
		btnDoExport.setText("Perform conversion");
		btnDoExport.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				Text importText = pswImport.getText();
				Text exportText = pswExport.getText();
				
				// perform conversion
				ModelConverterLogic model = ModelConverterLogic.getInstance();
				model.setImportPath(importText.getText());
				model.setExportPath(exportText.getText());
				model.performConversion();
				
				String dliPath = model.getDliPath();
				if(dliPath != null)
				{
					try {
						String dliContent = new String(Files.readAllBytes(Paths.get(dliPath)));
						
						MessageBox mb = new MessageBox(parent.getShell());
						mb.setText("Conversion successful");
						mb.setMessage("File saved to " + dliPath + " and .bin.");
						mb.open();
						
						textGlTF.setText(dliContent.toString());
						
						dliPath = new File(dliPath).getAbsolutePath();
						dliPath = dliPath.substring(0, dliPath.length() - ".dli".length());
						exportText.setText(dliPath);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// TODO Auto-generated method stub
				
			}
			
		});
		
		textGlTF = new StyledText(parent, SWT.BORDER | SWT.MULTI | SWT.V_SCROLL);
		textGlTF.setAlwaysShowScrollBars(false);
		textGlTF.setWordWrap(true);
		
		// do layout
		FormLayout layout = new FormLayout();
		parent.setLayout(layout);

		FormData importData = new FormData();
		importData.top = new FormAttachment(0, 0);
		pswImport.setLayoutData(importData);

		FormData exportData = new FormData();
		exportData.top = new FormAttachment(pswImport, 0);
		pswExport.setLayoutData(exportData);
		
		FormData btnData = new FormData();
		btnData.top = new FormAttachment(pswExport, 0);
		btnData.right = new FormAttachment(100, 0);
		btnDoExport.setLayoutData(btnData);
		
		FormData textData = new FormData();
		textData.left = new FormAttachment(0, 0);
		textData.right = new FormAttachment(100, 0);
		textData.top = new FormAttachment(btnDoExport, 0);
		textData.bottom = new FormAttachment(100, 0);
		textGlTF.setLayoutData(textData);
	}

	@Focus
	public void setFocus() {
		pswImport.getText().setFocus();
	}	
}
