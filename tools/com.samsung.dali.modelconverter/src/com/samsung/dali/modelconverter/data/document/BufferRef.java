package com.samsung.dali.modelconverter.data.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class BufferRef {

	@JsonProperty("byteOffset")
	public long mByteOffset;

	@JsonProperty("byteLength")
	public long mByteLength;
}
