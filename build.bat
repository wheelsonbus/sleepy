@echo off
rem Top-level build script

pushd engine
call build.bat
popd
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)

pushd playground
call build.bat
popd
if %ERRORLEVEL% neq 0 (echo "Error:"%ERRORLEVEL% && exit)

ECHO "Built all targets successfully."