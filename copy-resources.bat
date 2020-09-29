@echo off
set "DALI_RES=%DALI_APPLICATION_PACKAGE:/=\%"
xcopy %~dp0res %DALI_RES% /Y /E /S