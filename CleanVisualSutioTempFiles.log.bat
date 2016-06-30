REM echo %date%
REM Used in French Locale OS, /!\ date and time subparts extraction may not work regarding your local 
set year=%date:~-4%
set month=%date:~3,2%
set day=%date:~0,2%
REM echo %time%
set hour=%time:~0,2%
set min=%time:~3,2%
set secs=%time:~6,2%
cd ./
del /S /F /Q  *.obj;*.ilk;*.sdf;*.pch;*.pdb;*.tlog;*.bsc;*.sbr;*.idb; > CleanVisualSutioTempFiles_%year%%month%%day%_%hour%%min%%secs%.log
pause