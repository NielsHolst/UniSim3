if not exist output mkdir output
@echo Run client...
set cur_dir=%cd%
call %QT_ROOT_32%\qtenv2.bat
if exist "%cur_dir%\bin\igclientd.exe" (
  "%cur_dir%\bin32\igclientd.exe"
) else (
  "%cur_dir%\bin32\igclient.exe"
)
@echo Finished
pause