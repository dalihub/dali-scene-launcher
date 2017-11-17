package com.samsung.dali.modelConverter.model;

import java.io.IOException;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.samsung.dali.modelConverter.model.document.Document;

public class Project {

  public static Project sInstance;
  
  public static void create()
  {
    if(!isOpen())
    {
      sInstance = new Project();
    }
  }
  
  public static boolean isOpen()
  {
    return sInstance != null;
  }
  
  public static void close()
  {
    sInstance = null;
  }
  
  public String binLocation;
  public String dliLocation;
  
  public void deserialize(String dli) throws JsonParseException, JsonMappingException, IOException
  {
    ObjectMapper mapper = new ObjectMapper();
    mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
    
    document = mapper.readValue(dli, Document.class);
    document.resolveIds();
  }
  
  public String serialize() throws JsonProcessingException
  {
    document.updateIds();
    
    ObjectMapper mapper = new ObjectMapper();
    return mapper.writerWithDefaultPrettyPrinter().writeValueAsString(document);
  }
  
  public Document getDocument()
  {
    return document;
  }

  private Project()
  {}
  
  private Document document;
}  

