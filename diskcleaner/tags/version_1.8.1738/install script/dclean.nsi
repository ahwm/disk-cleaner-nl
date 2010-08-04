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
!packhdr tmp.dat "upx --brute tmp.dat"
;!endif

	Var /GLOBAL installer_name
	Var /GLOBAL outfile_target
	
;Modern UI Configuration
	!define MUI_ICON "main.ico"
	!define MUI_HEADERBITMAP "logo.bmp"
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
Icon main.ico

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
  !system "upx --brute ..\src\bin\Release\dclean.exe"
  File "..\src\bin\Release\dclean.exe"
  
  SetOutPath $INSTDIR\plug-ins

  File "plug-ins\*.dct"
; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner" "DisplayName" "Disk Cleaner (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DiskCleaner" "UninstallString" '"$INSTDIR\uninstall.exe"'

  WriteUninstaller "$INSTDIR\uninstall.exe"
 
;end
SectionEnd


;SubSection "!Plug-ins"
	
	
	; FindFirst $0 $1 *.dct
	; loop_start:
		; StrCmp $1 "" done
		
		; ReadINIStr $2 $1 "Info" "Title"
		; #ReadINIStr $3 $1 "Info" "Description"
		
		
		; Section $2
			
			; SectionIn 1
			; SetOutPath "$INSTDIR\plug-ins"
			; File "plug-ins\$1"

		; SectionEnd  
		
		; FindNext $0 $1
		; Goto loop_start
	; done:
	; FindClose $0

;SubSectionEnd


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
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\plug-ins\*.*"
  
  ; MUST REMOVE UNINSTALLER, too
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\Disk Cleaner\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\Disk Cleaner"
  RMDir "$INSTDIR\plug-ins"
  RMDir "$INSTDIR"
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
FunctionEnd