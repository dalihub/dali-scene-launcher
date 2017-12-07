package com.samsung.dali.modelexporter;

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

  static
  {
    System.loadLibrary("model-exporter-jni");
  }

  /**
   * @brief Performs model export, loading a .dae file, and writing .bin and
   *        .dli files.
   * @param inputFile - path to the .dae file to process. Required.
   * @param outputName - the name and path to save the .dli and .bin files to.
   *             Optional. Will use the input path and name if omitted.
   * @return 0 on success, 1 on failure.
   */
  public static native int nativeExport(String inputPath, String outputPath);

  /**
   * @brief Performs model conversion, loading a .dae file, and converting
   * 		it to the DLI format. In case of success, the dli and binary
   * 		contents can be retrieved by calling nativeGetDli/BinContents().
   * @param inputFile - path to the .dae file to process. Required.
   * @param binaryOutputName - the name and path to save the .bin file to.
   * @note Currently there's a restriction of having to save the .dli in the same
   *       location as the .bin file.
   * @return 0 on success, 1 on failure.
   */
  public static native int nativeConvert(String inputPath, String binaryOutputPath);

  /**
   * @return An explanation on what might have gone wrong (if anything);
   * 		null if everything is fine.
   * @note Consumes the error message.
   */
  public static native String nativeGetErrorMessage();

  /**
   * @return The path of the .dli file, only if nativeExport() was successful,
   * 		null otherwise.
   * @note Calling nativeExport() or nativeConvert() resets the string returned.
   */
  public static native String nativeGetDliPath();

  /**
   * @return The contents of the .dli after a successful call to nativeConvert(),
   * 		null if no successful calls have been made to nativeConvert().
   * @note Consumes the contents; subsequent calls will return null.
   */
  public static native String nativeGetDliContents();

  /**
   * @return The contents of the .bin after a successful call to nativeConvert(),
   * 		null if no successful calls have been made to nativeConvert().
   * @note Consumes the contents; subsequent calls will return null.
   */
  public static native byte[] nativeGetBinContents();
}
