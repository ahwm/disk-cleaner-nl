@echo off
REM - simple batch file to convert Launchpad structure to .MO folder structure to install

REM - remove old stuff and make new 'lang' folder
rd /S /Q lang 
mkdir lang

REM - run msgfmt command on all .po files -> compile to .mo
for %%D in (dclean plugins) do for %%X in (%%D\*.po) do (

REM - Compile .po to .mo
msgfmt "%%X" -o %%D\%%~nX%.mo
)

REM - move language file to own folder, such as dclean\es.mo -> lang\es\dclean.mo
for %%D in (dclean plugins) do for %%X in (%%D\*.mo) do (
if not exist lang\%%~nX mkdir lang\%%~nX
move %%X lang\%%~nX\%%~nD%.mo
)



