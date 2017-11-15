package com.samsung.dali.modelConverter.process;

public interface ILogText {
  void SetBusy( boolean isBusy );
  void AppendText( String str, int color );
  void ClearText();
}
