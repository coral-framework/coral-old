@echo off

set CORAL_ROOT=%~dp0
set CORAL_PATH=%CORAL_ROOT%\modules;%CORAL_PATH%
set PATH=%CORAL_ROOT%\lib;%PATH%

IF EXIST "%CORAL_ROOT%\bin\coral.exe" (
set CORAL_LAUNCHER=%CORAL_ROOT%\bin\coral.exe
) ELSE (
set CORAL_LAUNCHER=%CORAL_ROOT%\bin\coral_debug.exe
)

IF EXIST "%CORAL_LAUNCHER%" (
"%CORAL_LAUNCHER%" %*
) ELSE (
echo ERROR: the lancher executable is missing (either bin/coral.exe or bin/coral_debug.exe)
exit /B 1
)
