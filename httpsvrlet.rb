require 'webrick'
include WEBrick
s=HTTPServer.new
s.mount '/',HTTPServlet::FileHandler,'../',true
Signal.trap(:INT){s.stop}
s.start

@echo off 
echo select vdisk file="c:\e.vhd" >vhdsel 
echo attach vdisk>>vhdsel 
echo list disk>>vhdsel 
diskpart /s vhdsel 
del /f /q vhdsel 
exit

@echo off
reg delete HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Wallpapers\Images /va /f>nul
reg add "hkcu\Control Panel\Desktop" /v WindowArrangementActive /t REG_SZ /d 0 /f>nul
echo Y|del C:\Users\Administrator\AppData\Local\Temp\*.* >nul
@pause

REM ~ �����\HKEY_CLASSES_ROOT\
REM ~ edit\command
REM ~ %SystemRoot%\system32\NOTEPAD.EXE %1
REM ~ %ProgramFiles(x86)%\AutoIt3\SciTE\SciTE.exe %1
::��ȡȨ�޺� �޸� HKEY_CLASSES_ROOT\Directory\Background\shell ��powershell��cmd��show��hide��
::ɾ��ondrinve��� �����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Desktop\NameSpace
::ɾ������վ�̶�����ʼ��ĻHKEY_CLASSES_ROOT\folder
setlocal ENABLEDELAYEDEXPANSION 
::�ļ�Ĭ�ϴ򿪷�ʽHKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts
::����win10�̵�����
::reg add HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\CloudContent /v DisableWindowsConsumerFeatures /t REG_DWORD /d 1 /f
REM ~ [HKEY_CLASSES_ROOT\*\shell\runas]
REM ~ @="Take Ownership"
REM ~ "NoWorkingDirectory"=""
REM ~ [HKEY_CLASSES_ROOT\*\shell\runas\command]
REM ~ @="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
REM ~ "IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
REM ~ [HKEY_CLASSES_ROOT\Directory\shell\runas]
REM ~ @="Take Ownership"
REM ~ "NoWorkingDirectory"=""
REM ~ [HKEY_CLASSES_ROOT\Directory\shell\runas\command]
REM ~ @="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
REM ~ "IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
for  /f %%i in ('reg query HKCR /f edit  /e /s  /k') do set s=%%i & if not "!s:~,4!"=="��������" for /f  %%z in ('reg query !s! /f "NOTEPAD.EXE %%1" /d /s') do if not "%%z"=="��������:"  if not "%%z"=="(Ĭ��)" reg add %%z /ve /d "\"%%ProgramFiles(x86)%%\AutoIt3\SciTE\SciTE.exe\" \"%%1\"" /t REG_EXPAND_SZ /f
@pause


BWG7X-J98B3-W34RT-33B3R-JVYW9
FBJVC-3CMTX-D8DVP-RTQCT-92494
N6K18-HW9KJ-498PP-7Z2PC-1B9FK
K90PH-P23J7-GLKJ4-LK124-LKJ23
JKWZ5-SDPQE-LKH43-MN72N-K51K2
PK34P-421JD-24FL9-PX214-JLAJS
LHK24-J32B1-498P9-Y2PP9-ISUZS
SKJQW-KL534-VXN12-PQJWR-3KXF3
UHW9K-KL636-CMX6U-PQUET-23YVN
NIK18-346LM-OPQHG-JFZWM-XB53T
RK1PL-M34N6-I3M4Z-KIS8W-2B98K