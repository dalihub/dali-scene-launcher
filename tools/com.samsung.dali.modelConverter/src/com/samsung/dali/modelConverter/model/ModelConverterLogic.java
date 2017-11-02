package com.samsung.dali.modelConverter.model;

import java.io.File;

import com.samsung.dali.modelExporter.ModelExporter;

public class ModelConverterLogic {
	static {
		ModelExporter.initialise();
	}
	
	public static ModelConverterLogic getInstance()
	{
		if(sInstance == null)
		{
			sInstance = new ModelConverterLogic();
		}
		return sInstance;
	}
	
	public void setImportPath(String path)
	{
		importPath = path;
	}
	
	public void setExportPath(String path)
	{
		exportPath = path;
	}
	
	public void performConversion()
	{
		// can we read the source file?
		File in = new File(importPath);
		if(!in.exists() || !in.canRead())
		{
			throw new RuntimeException("Cannot read input: " + in.getAbsolutePath() + "!");
		}

		// if exportPath was not specified, try to get the import path, minus the .dae extension.
		if(exportPath.isEmpty())
		{
			exportPath = importPath.substring(0, importPath.length() - ".dae".length());
		}
		
		// if the target path exists, is it writable?
		File out = new File(exportPath);
		if(out.exists())
		{
			if(out.isFile())
			{
				out = out.getParentFile();
			}
	
			if(out == null || !out.canWrite())
			{
				throw new RuntimeException("Cannot write to folder: " + out.getAbsolutePath() + "!");
			}
		}

		int result = ModelExporter.nativeExport(in.getAbsolutePath(), out.getAbsolutePath());
		if(result != 0)
		{
			throw new RuntimeException(ModelExporter.nativeGetErrorMessage());
		}
	}
	
	public String getDliPath()
	{
		return ModelExporter.nativeGetDliPath();
	}

	private static ModelConverterLogic sInstance;

	private String importPath;
	private String exportPath;

	private ModelConverterLogic()
	{}
}
