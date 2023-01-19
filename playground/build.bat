rem Playground build script
@echo off
setlocal enabledelayedexpansion

rem Get all .c files
set sourceFilenames=
for /r %%f in (*.c) do (
    set cFilenames=!cFilenames! %%f
)
rem echo "Source files:" %cFilenames%

set target=playground
set compilerFlags=-g
set includeFlags=-Isrc -I../engine/src
set linkerFlags=-L../bin/ -lengine.lib
set definitions=-DZZ_DEBUG -DZZ_IMPORT

echo "Building %target%%..."
clang %cFilenames% %compilerFlags% -o ../bin/%target%.exe %definitions% %includeFlags% %linkerFlags%