@echo off
@echo Setting environment for using the GDAL Utilities.
set SDK_ROOT=%~dp0
set SDK_ROOT=%SDK_ROOT:\\=\%
SET "PATH=%SDK_ROOT%;%PATH%"
REM echo SDK_ROOT=%SDK_ROOT%
SET "GDAL_DATA=%SDK_ROOT%gdal-data"
ECHO GDAL_DATA=%GDAL_DATA%
REM SET "GDAL_DRIVER_PATH=%SDK_ROOT%gdalplugins"
SET "PROJ_LIB=%SDK_ROOT%projlib"
ECHO PROJ_LIB=%PROJ_LIB%
REM SET "PYTHONPATH=%SDK_ROOT%python"

test_dynamic_ogr2ogr -t_srs "IGNF:RGF93CC49" -f "MapInfo File" K:\###DataTest\Vecteur\TAB\FR_Arrond_2011.tab K:\###DataTest\Vecteur\SHP\FR_Arrond_2011.shp
pause