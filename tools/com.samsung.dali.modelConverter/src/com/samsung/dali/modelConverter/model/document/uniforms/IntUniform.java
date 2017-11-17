package com.samsung.dali.modelConverter.model.document.uniforms;

import com.samsung.dali.modelConverter.model.document.Uniform;
import com.samsung.dali.modelConverter.model.document.property.Property.IReceiver;

public class IntUniform extends Uniform {

  public IntUniform(int intValue) {
    value = intValue;
  }

  public int value;

  @Override
  public void registerProperties(IReceiver receiver) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public Object getValue() {

    return new Integer(value);
  }
}
