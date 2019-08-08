REM Remove Temp Visual Studio Files 
REM ******************************* 
 
del  /s   *.aps 
del  /s   *.bak
del  /s   *.clw 
del  /s   *.ncb 
del  /s   *.opt 
del  /s   *.plg 
del  /s   *.pdb 
del  /s   *.vsp 
del  /s   *.sdf 
del  /s   *.user
del  /s   /A -H *.suo 
del  /s   *.exp
del  /s   *.ilk 
 
del  /Q Debug\\*.* 
rmdir  /Q Debug 
del    /Q Release\\*.* 
rmdir  /Q Release 

for /r  %%a in (.) do @if exist %%a\Debug rd /s /q %%a\Debug
for /r  %%a in (.) do @if exist %%a\Release rd /s /q %%a\Release