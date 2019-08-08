@echo off

for /f "tokens=1-5 delims=|" %%i in ('dism /Online /Get-Drivers /Format:Table') do (
echo %%j|find /i "gevfilter" >nul && (
pnputil -d %%i
echo delete %%j succeed))

@echo off 
SETLOCAL ENABLEDELAYEDEXPANSION 

set var= 

for /f  "tokens=1-5 delims= " %%i in ('pnputil -e') do (
echo %%j|find /i "gev">nul && (
pnputil -d !var!
echo delete !var!)
echo %%j|findstr /c:"oem">nul 2>nul&&set var=%%j||set var=%%k
)

for /f  "tokens=1-5 delims= " %%i in ('pnputil -e') do (
echo %%j|find /i "Microvision">nul && (
pnputil -d !var!
echo delete !var!)
echo %%j|findstr /c:"oem">nul 2>nul&&set var=%%j||set var=%%k
)

