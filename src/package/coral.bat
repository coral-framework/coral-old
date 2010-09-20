@echo off
set CORAL_ROOT=%~dp0
set CORAL_PATH=%CORAL_ROOT%/modules;%CORAL_PATH%
set PATH=%CORAL_ROOT%/lib;%PATH%
%CORAL_ROOT%/bin/coral %*
