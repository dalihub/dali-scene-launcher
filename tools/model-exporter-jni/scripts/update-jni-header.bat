@echo off
pushd %~dp0

"%JAVA_HOME%\bin\javah" -classpath ..\..\com.samsung.dali.modelConverter\bin -o ..\include\NativeExporter.h com.samsung.dali.modelConverter.process.NativeExporter

popd