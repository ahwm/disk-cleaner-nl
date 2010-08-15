SetCompressor lzma
; dclean.nsi based on example2.nsi
;
; 
;
!include "MUI2.nsh"
!include WinVer.nsh
 
XPStyle on


RequestExecutionLevel admin

InstType Typical

;!ifdef HAVE_UPX
!packhdr tmp.dat "upx --lzma --best --all-filters tmp.dat"
;!endif

	Var /GLOBAL installer_name
	Var /GLOBAL outfile_target
	
;Modern UI Configuration
	!define MUI_ICON "../src/resource/main_new.ico"
	;!define MUI_HEADER_IMAGE
	;!define MUI_HEADER_IMAGE_BITMAP "logo.bmp"
	!define MUI_UNICON "del.ico"
	!define MUI_UNINSTALLER
	!define MUI_COMPONENTSPAGE_NODESC 

  	!insertmacro MUI_PAGE_WELCOME
	!define MUI_ABORTWARNING

  	!insertmacro MUI_PAGE_LICENSE "COPYING"
  	!insertmacro MUI_PAGE_COMPONENTS
  	!insertmacro MUI_PAGE_DIRECTORY
 	!insertmacro MUI_PAGE_INSTFILES
  	  !define MUI_FINISHPAGE_RUN "$INSTDIR\dclean.exe"
  	  !define MUI_FINISHPAGE_RUN_NOTCHECKED
 	  ;!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
  	  ;!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  	!insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  ;!insertmacro MUI_LANGUAGE "Dutch"


 Name $installer_name
 
	
OutFile target\dcsetup.exe

;Turn on CRC checking
CRCCheck on


; The default installation directory
InstallDir "$PROGRAMFILES\Disk Cleaner"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner" "UninstallString"

;Specify icon
Icon "../src/resource/main_new.ico"

;Compiler flags
SetOverwrite ifnewer
SetCompress force
SetDatablockOptimize on

; The stuff to install
Section "!Application"


SectionIn 1

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
 
  ;Put files there
  !system "upx --best --lzma --all-filters --compress-icons=0 ..\src\bin\Release\dclean.exe"
  File "..\src\bin\Release\dclean.exe"
  
  SetOutPath $INSTDIR\plug-ins

  File "..\plug-ins\*.dct"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner" "DisplayName" "Disk Cleaner (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner" "UninstallString" '"$INSTDIR\uninstall.exe"'

  WriteUninstaller "$INSTDIR\uninstall.exe"


 SectionEnd
 
 Section "!Additional languages"
SectionIn 1
 ; Create the .mo folder structure
 !cd ..\languages
 !system po2mo.bat
 !cd "..\install script"
  SetOutPath $INSTDIR\lang
  File /r ..\languages\lang\*.*
  
;end
SectionEnd


SubSection "!Shortcuts"

; optional section program shortcuts
Section "Create Program Shortcuts in Start Menu"
  SetShellVarContext all
  SectionIn 1
  CreateDirectory "$SMPROGRAMS\Disk Cleaner"
  CreateShortCut "$SMPROGRAMS\Disk Cleaner\Disk Cleaner.lnk" "$INSTDIR\dclean.exe" "" "$INSTDIR\dclean.exe" 0
  CreateShortCut "$SMPROGRAMS\Disk Cleaner\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0   
 
SectionEnd



;Optional Section quick launch
Section "Create Quick Launch Shortcut"
  SectionIn 1
  StrCmp $QUICKLAUNCH $TEMP NO
  CreateShortCut "$QUICKLAUNCH\Disk Cleaner.lnk" "$INSTDIR\dclean.exe" "" \
  "$INSTDIR\dclean.exe" 0
NO:
SectionEnd

SubSectionEnd

;Display the Finish header
;Insert this macro after the sections if you are not using a finish page
;!insertmacro MUI_PAGE_FINISH

; uninstall stuff

; special uninstall section.
Section "Uninstall"
  SetShellVarContext all
  
  ; remove registry keys
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Disk Cleaner"
  ; remove all files
  Delete /REBOOTOK "$INSTDIR\dclean.exe"
   
  
  ; MUST REMOVE UNINSTALLER, too
  Delete /REBOOTOK "$INSTDIR\uninstall.exe"
  
  ; remove directories used.
  RMDir /r /REBOOTOK "$SMPROGRAMS\Disk Cleaner"
  RMDir /r /REBOOTOK "$INSTDIR\plug-ins"
  RMDir /r /REBOOTOK "$INSTDIR\lang"
  RMDir "$INSTDIR" ;will only work if empty. User's files are safe
  Delete "$QUICKLAUNCH\Disk Cleaner.lnk"
  
SectionEnd

; eof

Function .onInit
	
  ${IfNot} ${AtLeastWinXP}
    MessageBox MB_OK "Sorry, this application will not run on you computer. XP and above are required."
    Quit
  ${EndIf}
 	
	GetDllVersionLocal "../src/bin/Release/dclean.exe" $R0 $R1
	IntOp $R2 $R0 / 0x00010000
	IntOp $R3 $R0 & 0x0000FFFF
	IntOp $R4 $R1 / 0x00010000
	IntOp $R5 $R1 & 0x0000FFFF
	StrCpy $installer_name "Disk Cleaner $R2.$R3.$R4"
	
	StrCpy $outfile_target "target\dcsetup_$R2.$R3.$R4.exe"
	
	;!insertmacro MUI_LANGDLL_DISPLAY
	
FunctionEnd


; Function un.onInit

  ; !insertmacro MUI_UNGETLANGUAGE
  
; FunctionEnd