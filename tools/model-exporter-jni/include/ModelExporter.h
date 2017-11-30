#ifndef MODELEXPORTER_H_
#define MODELEXPORTER_H_

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

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jint JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeExport(
    JNIEnv* env, jclass clazz, jstring inputFile, jstring outputFile);

JNIEXPORT jint JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeConvert(
    JNIEnv* env, jclass clazz, jstring inputFile, jstring binaryOutputFile);

JNIEXPORT jstring JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeGetErrorMessage(
    JNIEnv* env, jclass clazz);

JNIEXPORT jstring JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeGetDliPath(
    JNIEnv* env, jclass clazz);

JNIEXPORT jstring JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeGetDliContents(
    JNIEnv* env, jclass clazz);

JNIEXPORT jbyteArray JNICALL Java_com_samsung_dali_modelexporter_ModelExporter_nativeGetBinContents(
    JNIEnv* env, jclass clazz);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MODELEXPORTER_H_ */
