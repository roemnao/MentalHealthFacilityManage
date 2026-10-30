@echo off
rem Tim MSVC bang vswhere, nap moi truong roi bien dich main.cpp
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Chua cai MSVC. Hay cai Build Tools for Visual Studio - workload "Desktop development with C++".
    exit /b 1
)
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo Khong tim thay MSVC C++ tools.
    exit /b 1
)
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
cd /d "%~dp0"
cl /nologo /EHsc /W3 /Zi /D_CRT_SECURE_NO_WARNINGS /Fe:main.exe /Fo:main.obj main.cpp
