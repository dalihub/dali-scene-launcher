package com.samsung.dali.modelConverter.model.document.uniforms;

import com.samsung.dali.modelConverter.model.document.Uniform;
import com.samsung.dali.modelConverter.model.document.property.Property.IReceiver;

public class Vec4Uniform extends Uniform {

  public Vec4Uniform(Number[] numbers) {
    assert numbers.length == 4;
    value = numbers;
  }
  
  Number[] value;

  @Override
  public void registerProperties(IReceiver receiver) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public Object getValue() {

    return value;
  }
}
