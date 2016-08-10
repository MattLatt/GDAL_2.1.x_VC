REM echo %date%
REM Used in French Locale Windows, /!\ date and time subparts extraction index may have to be updated regarding the windows locale
REM Extracting the Date
set year=%date:~-4%
set month=%date:~3,2%
set day=%date:~0,2%
REM Space replacement with 0
REM set day=%day: =0%

REM Extracting the Time
set hour=%time:~0,2%
REM Space replacement with 0
set hour=%hour: =0%
set min=%time:~3,2%
set secs=%time:~6,2%
cd ./
del /S /F /Q  *.obj;*.ilk;*.sdf;*.pch;*.pdb;*.tlog;*.bsc;*.sbr;*.idb; > CleanVisualStudioTempFiles_%year%%month%%day%_%hour%%min%%secs%.log
pause