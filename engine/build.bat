rem Engine build script
@echo off
setlocal enabledelayedexpansion

rem Get all .c files
set sourceFilenames=
for /r %%f in (*.c) do (
    set cFilenames=!cFilenames! %%f
)
rem echo "Source files:" %cFilenames%

set target=engine
set compilerFlags=-g -shared -Wvarargs -Wall -Werror
set includeFlags=-Isrc -I%VULKAN_SDK%/Include
set linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
set defines=-D_DEBUG -DZZ_EXPORT -D_CRT_SECURE_NO_WARNINGS

echo "Building %target%%..."
clang %cFilenames% %compilerFlags% -o ../bin/%target%.dll %defines% %includeFlags% %linkerFlags%