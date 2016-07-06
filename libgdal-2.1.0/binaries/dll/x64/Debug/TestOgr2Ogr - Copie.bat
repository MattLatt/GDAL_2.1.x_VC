@echo off
@echo Setting environment for using the GDAL Utilities.
REM set SDK_ROOT=%~dp0
REM set SDK_ROOT=%SDK_ROOT:\\=\%
REM SET "PATH=%SDK_ROOT%;%PATH%"
REM echo SDK_ROOT=%SDK_ROOT%
REM SET "GDAL_DATA=%SDK_ROOT%gdal-data"
REM SET "GDAL_DRIVER_PATH=%SDK_ROOT%gdalplugins"
REM SET "PROJ_LIB=%SDK_ROOT%projlib"
REM SET "PYTHONPATH=%SDK_ROOT%python"

test_dynamic_ogr2ogr -t_srs "IGNF:RGF93CC49" -f "MapInfo File" K:\###DataTest\Vecteur\TAB\FR_Arrond_2011.tab K:\###DataTest\Vecteur\SHP\FR_Arrond_2011.shp
pause