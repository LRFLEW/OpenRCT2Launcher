Name "OpenRCT2 Launcher"
OutFile "build-OpenRCT2Launcher-Distribute-Archive\OpenRCT2Launcher.exe"
InstallDir $PROGRAMFILES\OpenRCT2Launcher
InstallDirRegKey HKLM "Software\OpenRCT2Launcher" "Install_Dir"
RequestExecutionLevel admin

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

!system "rmdir /S /Q build-OpenRCT2Launcher-Distribute-Unpacked & mkdir build-OpenRCT2Launcher-Distribute-Unpacked"
!system "pushd build-OpenRCT2Launcher-*32bit-Release & copy release\OpenRCT2.exe ..\build-OpenRCT2Launcher-Distribute-Unpacked\OpenRCT2.exe & popd"
!system "windeployqt --dir build-OpenRCT2Launcher-Distribute-Unpacked\OpenRCT2Launcher-Qt5 build-OpenRCT2Launcher-Distribute-Unpacked\OpenRCT2.exe"
!system "copy C:\OpenSSL-Win32\libeay32.dll build-OpenRCT2Launcher-Distribute-Unpacked\libeay32.dll"
!system "copy C:\OpenSSL-Win32\ssleay32.dll build-OpenRCT2Launcher-Distribute-Unpacked\ssleay32.dll"
!system "rmdir /S /Q build-OpenRCT2Launcher-Distribute-Archive & mkdir build-OpenRCT2Launcher-Distribute-Archive"
!system "pushd build-OpenRCT2Launcher-Distribute-Unpacked & 7z a -tzip ../build-OpenRCT2Launcher-Distribute-Archive\OpenRCT2Launcher.zip * & popd"

Section "OpenRCT2 Launcher"
  SectionIn RO
  SetOutPath $INSTDIR
  File /r "build-OpenRCT2Launcher-Distribute-Unpacked\OpenRCT2.exe"
  File /r "build-OpenRCT2Launcher-Distribute-Unpacked\libeay32.dll"
  File /r "build-OpenRCT2Launcher-Distribute-Unpacked\ssleay32.dll"
  File /r "build-OpenRCT2Launcher-Distribute-Unpacked\OpenRCT2Launcher-Qt5"
  WriteRegStr HKLM "Software\OpenRCT2Launcher" "Install_Dir" "$INSTDIR"
  CreateShortcut "$SMPROGRAMS\OpenRCT2.lnk" "$INSTDIR\OpenRCT2.exe"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenRCT2Launcher" "DisplayName" "OpenRCT2 Launcher"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenRCT2Launcher" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenRCT2Launcher" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenRCT2Launcher" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
SectionEnd

Section "Desktop Shortcut"
  CreateShortcut "$DESKTOP\OpenRCT2.lnk" "$INSTDIR\OpenRCT2.exe"
SectionEnd

Section "Uninstall"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenRCT2Launcher"
  DeleteRegKey HKLM "Software\OpenRCT2Launcher"
  Delete "$INSTDIR\OpenRCT2.exe"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"
  RMDir /r "$INSTDIR\OpenRCT2Launcher-Qt5"
  Delete "$SMPROGRAMS\OpenRCT2.lnk"
  Delete "$DESKTOP\OpenRCT2.lnk"
SectionEnd
