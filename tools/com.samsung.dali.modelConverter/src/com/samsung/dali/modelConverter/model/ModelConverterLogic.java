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
		mImportPath = path;
	}

	public void setExportPath(String path)
	{
		mExportPath = path;
	}

	public String performConversion()
	{
		// can we read the source file?
		File in = new File(mImportPath);
		if(!in.exists() || !in.canRead())
		{
			throw new RuntimeException("Cannot read input: " + in.getAbsolutePath() + "!");
		}

		// if exportPath was not specified, try to get the import path, minus the .dae extension.
		if(mExportPath.isEmpty())
		{
			mExportPath = mImportPath.substring(0, mImportPath.length() - ".dae".length());
		}

		// if the target path exists, is it writable?
		File out = new File(mExportPath);
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

		String result = ModelExporter.nativeConvert(in.getAbsolutePath(), out.getAbsolutePath());
		if(result == null)
		{
			throw new RuntimeException(ModelExporter.nativeGetErrorMessage());
		}
		return result;
	}

	private static ModelConverterLogic sInstance;

	private String mImportPath;
	private String mExportPath;

	private ModelConverterLogic()
	{}
}
