package com.samsung.dali.modelConverter.model.document;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Material implements Document.IdResolver {
  
  @JsonProperty("texture1")
  public String albedoMetal;

  @JsonProperty("texture2")
  public String normalRough;
  
  @JsonProperty("environment")
  public int environmentId;
  
  @JsonIgnore
  public Environment environment;

  @Override
  public void resolveIds(Document doc) {

    environment = doc.environments.get(environmentId);
  }

  @Override
  public void updateIds(Document doc) {
    
    environmentId = doc.environments.indexOf(environment);
  }

}
