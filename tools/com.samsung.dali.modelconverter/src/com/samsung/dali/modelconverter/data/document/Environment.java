package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Environment {

	public String getSpecularPath() {
		return mSpecularPath;
	}

	public void setSpecularPath(String path) {
		mSpecularPath = path;
	}

	public String getDiffusePath() {
		return mDiffusePath;
	}

	public void setDiffusePath(String path) {
		mDiffusePath = path;
	}

	@JsonProperty("cubeSpecular")
	private String mSpecularPath;

	@JsonProperty("cubeDiffuse")
	private String mDiffusePath;
}
