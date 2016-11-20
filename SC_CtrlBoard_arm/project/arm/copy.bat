@echo off
copy /Y ..\..\bin\obj\*.axf f:\work\bin\axf\
copy /Y ..\..\bin\obj\*.hex f:\work\bin
copy /Y ..\..\bin\obj\*.bin f:\work\bin

:: fromelf.exe --bin --output f:/work/bin/%1.bin f:/work/bin/axf/%1.axf
exit