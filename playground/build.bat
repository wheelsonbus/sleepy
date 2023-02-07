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
set compilerFlags=-g -shared -Wvarargs -Wall -Werror -Wno-missing-braces
set includeFlags=-Iinclude -I../zz/include
set linkerFlags=-L../bin/ -lzz.lib
set definitions=-DPLAYGROUND_EXPORT -DZZ_DEBUG

echo "Building %target%%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%target%.dll %includeFlags% %linkerFlags% %definitions%