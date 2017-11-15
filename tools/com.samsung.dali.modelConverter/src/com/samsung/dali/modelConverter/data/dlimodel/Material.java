package com.samsung.dali.modelConverter.data.dlimodel;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

public class Material implements DliModel.IdResolver {
  
  @JsonProperty("texture1")
  public String albedoMetal;

  @JsonProperty("texture2")
  public String normalRough;
  
  @JsonProperty("environment")
  public int environmentId;
  
  @JsonIgnore
  public Environment environment;

  @Override
  public void resolveIds(DliModel doc) {

    environment = doc.environments.get(environmentId);
  }

  @Override
  public void updateIds(DliModel doc) {
    
    environmentId = doc.environments.indexOf(environment);
  }

}
