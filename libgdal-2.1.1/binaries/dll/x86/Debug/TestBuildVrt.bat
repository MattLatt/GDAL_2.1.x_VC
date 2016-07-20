@echo off
@echo Setting environment for using the GDAL Utilities.
set SDK_ROOT=%~dp0
set SDK_ROOT=%SDK_ROOT:\\=\%
echo %SDK_ROOT%
SET "PATH=%SDK_ROOT%;%PATH%"
SET "GDAL_DATA=%SDK_ROOT%gdal-data"
echo %GDAL_DATA%
SET "GDAL_DRIVER_PATH=%SDK_ROOT%gdalplugins"
echo %GDAL_DRIVER_PATH%
SET "PROJ_LIB=%SDK_ROOT%projlib"
echo %PROJ_LIB%
SET "PYTHONPATH=%SDK_ROOT%python"
echo %PYTHONPATH%

REM test_dynamic_gdalbuildvrt 
pause