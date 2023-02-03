set cur_dir=%cd%
set xmlfile="C:/Users/au152367/Documents/QDev/UniSim3/input/projects/vg blank.x/2022-10-05-NyXML.xml"
set outputfile="C:/Users/au152367/Documents/QDev/UniSim3/output 2.x/vg.txt"
call %QT_ROOT%\qtenv2.bat
if exist "%cur_dir%\bin\unisimd.exe" (
  %cur_dir%\bin\unisimd.exe run %xmlfile% %outputfile%
) else (
  %cur_dir%\bin\unisim.exe run %xmlfile% %outputfile%
)
cd %cur_dir%
pause 