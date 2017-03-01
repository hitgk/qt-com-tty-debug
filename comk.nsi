; GCommQT.nsi
;--------------------------------

; The name of the installer
Name "Qt_comport"

; The file to write
OutFile "Qt_comportSetup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Qt_comport

; Request application privileges for Windows Vista
RequestExecutionLevel user

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Gorin\Qt_comport" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

LoadLanguageFile "${NSISDIR}\Contrib\Language files\Russian.nlf"

; The stuff to install
Section "qt_comport.exe (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File  "Release\Qt_comport.exe"
  File /r "D:\Documents\Work\MyPrograms\VC2005dllPacket\"
  File "C:\qt4\4.4.3\bin\QtCore4.dll"
  File "C:\qt4\4.4.3\bin\QtGui4.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Gorin\Qt_comport "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Gorin\Qt_comport" "DisplayName" "Qt_comport"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Gorin\Qt_comport" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Gorin\Qt_comport" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Gorin\Qt_comport" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Qt_comport"
  CreateShortCut "$SMPROGRAMS\Qt_comport\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Qt_comport\Qt_comport.lnk" "$INSTDIR\Qt_comport.exe" "" "$INSTDIR\Qt_comport.exe" 0
  
SectionEnd

; Optional section (can be disabled by the user)
Section "DESKTOP Shortcut"

  CreateShortCut "$DESKTOP\Qt_comport.lnk" "$INSTDIR\Qt_comport.exe" "" "$INSTDIR\Qt_comport.exe" 0
  
SectionEnd
;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Gorin\Qt_comport"
  DeleteRegKey HKLM SOFTWARE\Gorin\Qt_comport

  ; Remove files and uninstaller
  Delete $INSTDIR\Qt_comport.exe
  Delete $INSTDIR\Qt_comport.ini  
  Delete $INSTDIR\uninstall.exe
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Qt_comport\*.*"
  Delete "$DESKTOP\Qt_comport.lnk"
  
  ; Remove directories used
  RMDir "$SMPROGRAMS\Qt_comport"
  RMDir /r "$INSTDIR"

SectionEnd
