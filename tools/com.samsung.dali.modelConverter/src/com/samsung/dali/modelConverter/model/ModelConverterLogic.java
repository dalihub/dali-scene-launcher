package com.samsung.dali.modelConverter.model;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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
