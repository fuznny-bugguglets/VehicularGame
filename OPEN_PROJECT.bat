@echo off
setlocal

REM Get the directory where this batch file is located
set "SCRIPT_DIR=%~dp0"

REM Full path to temp folders
set "FOLDER1=%SCRIPT_DIR%Binaries"
set "FOLDER2=%SCRIPT_DIR%DerivedDataCache"
set "FOLDER3=%SCRIPT_DIR%Intermediate"
set "FOLDER4=%SCRIPT_DIR%Saved"

REM Check folders and then delete
if exist "%FOLDER1%" (
	rmdir /s /q "%FOLDER1%"
) 
if exist "%FOLDER2%" (
	rmdir /s /q "%FOLDER2%"
)
if exist "%FOLDER3%" (
	rmdir /s /q "%FOLDER3%"
)
if exist "%FOLDER4%" (
	rmdir /s /q "%FOLDER4%"
)

start "" "%SCRIPT_DIR%VehicularGame.uproject"

endlocal