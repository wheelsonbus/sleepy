rem Client build script
@echo off
setlocal enabledelayedexpansion

rem Get all .c files
set sourceFiles=
for /r %%f in (*.c) do (
    set sourceFiles=!sourceFiles! %%f
)
rem echo "Source files:" %sourceFiles%

set target=server
set compilerFlags=-g
set includeFlags=-I../zz/include -I../playground/include
set linkerFlags=-L../bin/ -lzz.lib -lplayground.lib
set definitions=-DZZ_LITTLE_ENDIAN -DZZ_DEBUG

echo "Building %target%%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%target%.exe %includeFlags% %linkerFlags% %definitions%