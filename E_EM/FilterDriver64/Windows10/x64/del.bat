@echo off
for /f "tokens=1-5 delims=|" %%i in ('dism /Online /Get-Drivers /Format:Table') do (
echo %%j|find /i "gevfilter" >nul && (
pnputil -d %%i
echo delete %%j succeed))

for /f  "tokens=1-5 delims= " %%i in ('pnputil /enum-drivers') do (
echo %%j|find /i "gevfilter" >nul && (
pnputil /delete-driver !var!
echo delete !var!)
echo %%j|findstr /c:"oem">nul 2>nul&&set var=%%j||set var=%%k
)
