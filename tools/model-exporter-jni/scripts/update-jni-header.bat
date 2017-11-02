@echo off
pushd %~dp0

"%JAVA_HOME%\bin\javah" -classpath ..\..\com.samsung.dali.modelConverter\bin -o ..\include\ModelExporter.h com.samsung.dali.modelExporter.ModelExporter

popd