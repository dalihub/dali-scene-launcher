package com.samsung.dali.modelConverter.process;

public class NativeExporter {
  
  static
  {
      System.loadLibrary("model-exporter-jni");
  }

  /**
   * @brief Performs the export.
   * @param inputFile - the .dae file to process. Required.
   * @param outputName - the name and path to save the .dli and .bin files.
   *            Optional. Will use the input path and name if omitted.
   * @return 0 on success, 1 on failure.
   */
  public static native int Export(String inputFile, String outputName);

  /**
   * @return An explanation on what might have gone wrong, if anything has.
   *      Null string if everything is fine.
   * @note Consumes the error message.
   */
  public static native String GetErrorMessage();

  /**
   * @return The path of the .dli file, if only nativeExport() was successful.
   * @note Calling nativeExport resets the string returned.
   */
  public static native String GetDliPath();
}
