package com.samsung.dali.modelConverter.model.document;

import com.fasterxml.jackson.annotation.JsonProperty;

public class BufferRef {

  @JsonProperty("byteOffset")
  public long byteOffset;
  
  @JsonProperty("byteLength")
  public long byteLength;
  
}
