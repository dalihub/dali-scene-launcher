package com.samsung.dali.modelConverter.data.dlimodel;

import java.util.ArrayList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.samsung.dali.modelConverter.data.dlimodel.uniforms.IntUniform;
import com.samsung.dali.modelConverter.data.dlimodel.uniforms.MatrixUniform;
import com.samsung.dali.modelConverter.data.dlimodel.uniforms.Vec4Uniform;


public class Shader {

  @JsonProperty("vertex")
  public String vertexPath;

  @JsonProperty("fragment")
  public String fragmentPath;

  @JsonProperty("renderMode")
  public int renderMode;
  
  @JsonIgnore
  public TreeMap<String, Uniform> uniforms;

  @JsonAnySetter
  public void add(String key, Object value)
  {
    if (uniforms == null)
    {
      uniforms = new TreeMap<String, Uniform>();
    }
    
    if(value instanceof Integer)
    {
      uniforms.put(key, new IntUniform(((Integer)value).intValue()) );
    }
    else if(value instanceof ArrayList<?>)
    {
      ArrayList<?> arrayList = (ArrayList<?>)value;
      
      if(arrayList.isEmpty())
      {
        throw new IllegalArgumentException("Failed to create uniform from empty array. What type?");
      }

      for(Object o: arrayList)
      {
        if(!(o instanceof Number))
        {
          throw new IllegalArgumentException("Failed to create uniform from non-numeric type " +
            o.getClass().getName() + " @ index " + arrayList.indexOf(o));
        }
      }
      
      switch(arrayList.size())
      {
      case 4:
        uniforms.put(key, new Vec4Uniform(arrayList.toArray(new Number[0])));
        break;
        
      case 16:
        uniforms.put(key, new MatrixUniform(arrayList.toArray(new Number[0])));
        break;
        
      default:
        throw new IllegalArgumentException("Unsupported number of elements in uniform: " + arrayList.size());
      }
    }
    else
    {
      throw new IllegalArgumentException("Unknown type: " + value.getClass().getName());
    }
  }
  
  @JsonAnyGetter
  public Map<String, Object> get()
  {
    Map<String, Object> values = new TreeMap<String, Object>();
    for(Entry<String, Uniform> u : uniforms.entrySet())
    {
      values.put(u.getKey(), u.getValue().getValue());
    }
    return values;
  }

}
