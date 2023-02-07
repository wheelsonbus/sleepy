@echo off
rem Top-level build script

pushd zz
call build.bat
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)
popd

pushd playground
call build.bat
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)
popd

pushd server
call build.bat
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)
popd

pushd client
call build.bat
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)
popd

ECHO "Built all targets successfully."