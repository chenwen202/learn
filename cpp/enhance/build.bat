@echo off
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
)
set compilerflags=/Od /Zi /EHsc /std:c++latest /I include  

set linkerflags=/OUT:bin\main.exe user32.lib Gdi32.lib
cl.exe %compilerflags% src\*.cpp /SUBSYSTEM:WINDOWS  /link %linkerflags%  /DYNAMICBASE 
del bin\*.ilk *.obj *.pdb