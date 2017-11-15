package com.samsung.dali.modelConverter.data;

import java.io.IOException;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.samsung.dali.modelConverter.data.dlimodel.*;

public class Project {
  
  private  DliModel dlimodel;
  
  //TODO:Move all the static vars to GlobalData.
  public static Project sInstance;
  
  public static void create()
  {
     sInstance = new Project();
  }
  
  
  public void JsonLoad(String dli) throws JsonParseException, JsonMappingException, IOException
  {
    ObjectMapper mapper = new ObjectMapper();
    mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
    
    dlimodel = mapper.readValue(dli, DliModel.class);
    dlimodel.resolveIds();
  }
  
  public String JsonSave() throws JsonProcessingException
  {
    dlimodel.updateIds();
    
    ObjectMapper mapper = new ObjectMapper();
    return mapper.writerWithDefaultPrettyPrinter().writeValueAsString( dlimodel );
  }


  public DliModel getDliModel() {
    return dlimodel; 
  }
  
}
