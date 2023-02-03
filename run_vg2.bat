set cur_dir=%cd%
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bin\unisimd.exe" (
  %cur_dir%\bin\unisimd.exe
) else (
  %cur_dir%\bin\unisim.exe run C:/Users/au152367/Documents/QDev/UniSim3/input/projects/vg/2022-03-23-Standard.xml C:/Users/au152367/Documents/QDev/UniSim3/output/vg.txt
)
cd %cur_dir%
pause 