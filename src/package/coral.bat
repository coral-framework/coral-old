@echo off

set CORAL_ROOT=%~dp0
set CORAL_PATH=%CORAL_ROOT%\modules;%CORAL_PATH%
set PATH=%CORAL_ROOT%\lib;%PATH%

IF EXIST "%CORAL_ROOT%\bin\launcher.exe" (
set CORAL_LAUNCHER=%CORAL_ROOT%\bin\launcher.exe
) ELSE (
set CORAL_LAUNCHER=%CORAL_ROOT%\bin\launcher_debug.exe
)

IF EXIST "%CORAL_LAUNCHER%" (
"%CORAL_LAUNCHER%" %*
) ELSE (
echo "ERROR: could not find the launcher executable (tried bin/launcher.exe and bin/launcher_debug.exe)"
exit /B 1
)
