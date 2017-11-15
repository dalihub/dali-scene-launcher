package com.samsung.dali.modelExporter;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

public class ModelExporter {

  private static boolean s_initialised = false;

  public static void initialise()
  {
    if(!s_initialised)
    {
      System.loadLibrary("model-exporter-jni");
      s_initialised = true;
    }
  }

  /**
   * @brief Performs the export.
   * @param inputFile - the .dae file to process. Required.
   * @param outputName - the name and path to save the .dli and .bin files.
   * 						Optional. Will use the input path and name if omitted.
   * @return 0 on success, 1 on failure.
   */
  public static native int nativeExport(String inputFile, String outputName);

  /**
   * @return An explanation on what might have gone wrong, if anything has.
   * 			Null string if everything is fine.
   * @note Consumes the error message.
   */
  public static native String nativeGetErrorMessage();

  /**
   * @return The path of the .dli file, if only nativeExport() was successful.
   * @note Calling nativeExport resets the string returned.
   */
  public static native String nativeGetDliPath();
}
