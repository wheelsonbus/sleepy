rem Engine build script
@echo off
setlocal enabledelayedexpansion

rem Get all .c files
set sourceFiles=
for /r %%f in (*.c) do (
    set sourceFiles=!sourceFiles! %%f
)
rem echo "Source files:" %sourceFiles%

set target=zz
set compilerFlags=-g -shared -Wvarargs -Wall -Werror -Wno-missing-braces
set includeFlags=-Iinclude -I%VULKAN_SDK%/Include
set linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
set definitions=-DZZ_WINDOWS -DZZ_VULKAN -DZZ_DEBUG -DZZ_EXPORT -D_CRT_SECURE_NO_WARNINGS

echo "Building %target%%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%target%.dll %includeFlags% %linkerFlags% %definitions%