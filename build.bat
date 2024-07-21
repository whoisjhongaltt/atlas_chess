@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
del *.pdb > NUL 2> NUL
cl /Zi raylib.lib Shell32.lib User32.lib gdi32.lib msvcrt.lib  winmm.lib ..\main.c /I..\raylib\include /I..\raygui\src /link /libpath:..\raylib\lib /NODEFAULTLIB:libcmt
popd