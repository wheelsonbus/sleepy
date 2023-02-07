rem Playground build script
@echo off
setlocal enabledelayedexpansion

rem Get all .c files
set sourceFiles=
for /r %%f in (*.c) do (
    set sourceFiles=!sourceFiles! %%f
)
rem echo "Source files:" %sourceFiles%

set target=playground
set compilerFlags=-g
set includeFlags=-Isrc -I../zz/include -I../zz/src -I%VULKAN_SDK%/Include
set linkerFlags=-L../bin/ -lzz.lib
set definitions=-DZZ_WINDOWS -DZZ_VULKAN -DZZ_DEBUG -DZZ_IMPORT

echo "Building %target%%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%target%.exe %includeFlags% %linkerFlags% %definitions%