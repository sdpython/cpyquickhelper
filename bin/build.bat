@echo off
set current=%~dp0
set root=%current%..
cd %root%
@echo ##################
@echo Compile
@echo running %root%\setup.py build_ext --inplace
@echo ##################
C:\Python36_x64\python %root%\setup.py build_ext --inplace
if %errorlevel% neq 0 exit /b %errorlevel%
@echo Done Compile.
@echo ##################
@echo Build
@echo running %root%\setup.py bdist_wheel
@echo ##################
C:\Python36_x64\python %root%\setup.py bdist_wheel
if %errorlevel% neq 0 exit /b %errorlevel%
@echo Done Build.
cd %current%