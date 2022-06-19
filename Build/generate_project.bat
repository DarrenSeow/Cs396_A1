@echo off
setlocal enabledelayedexpansion

:DOWNLOAD_DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

rmdir "../dependencies/freeglut" /S /Q
git clone https://github.com/FreeGLUTProject/freeglut.git "../dependencies/freeglut"
if %ERRORLEVEL% GEQ 1 goto :PAUSE

echo.
rmdir "../dependencies/freeglut_lib_bin" /S /Q
git clone https://github.com/hotaru08/freeglut_lib_bin.git "../dependencies/freeglut_lib_bin"
if %ERRORLEVEL% GEQ 1 goto :PAUSE



:DONE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DONE!!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
goto :PAUSE

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red ERROR!!
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------

:PAUSE
rem if no one give us any parameters then we will pause it at the end, else we are assuming that another batch file called us
if %1.==. pause
