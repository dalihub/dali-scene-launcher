package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Material {

	public String getAlbedoMetal() {
		return mAlbedoMetal;
	}

	public void setAlbedoMetal(String albedoMetal) {
		this.mAlbedoMetal = albedoMetal;
	}

	public String getNormalRough() {
		return mNormalRough;
	}

	public void setNormalRough(String normalRough) {
		mNormalRough = normalRough;
	}

	public Integer getEnvironmentId() {
		return mEnvironmentId;
	}

	public void setEnvironmentId(Integer environmentId) {
		mEnvironmentId = environmentId;
	}

	public Environment getEnvironment(Document doc) {
		return getEnvironmentId() != null ? doc.getEnvironments().get(getEnvironmentId()) : null;
	}

	@JsonProperty("texture1")
	private String mAlbedoMetal;

	@JsonProperty("texture2")
	private String mNormalRough;

	@JsonProperty("environment")
	private Integer mEnvironmentId;

}
