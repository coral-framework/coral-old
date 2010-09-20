@echo off
set CORAL_ROOT=%~dp0
"%CORAL_ROOT%/coral" lua.Launcher co.compiler.cli %*
