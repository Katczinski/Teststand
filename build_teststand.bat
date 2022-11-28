setlocal EnableDelayedExpansion
echo off
Rem ==========================setting variables==========================
set OpenSSL="external\OpenSSL-Win32\bin\"
set d2xx="external\D2xx\"
set release="release\"
set qtBin="C:\Qt\5.15.2\mingw81_32\bin"
set mans_read="release\Teststand.exe"

for %%a IN ("%PATH:;=" "%") do (
	set str=%%a
	if NOT "!str!"=="!str:Qt=!" (
		set qtBin=!str!
	)
)
Rem ==========================building project===========================
echo on
qmake Teststand.pro
mingw32-make -f Makefile.Release
for %%f in (!OpenSSL!*.dll) DO copy "%%f" !release!
copy !d2xx!D2xx.dll !release!
xcopy ..\flash !release!flash /s /e /y /I
xcopy ..\scripts !release!scripts /s /e /y /I
xcopy ..\images !release!images /s /e /y /I
for %%f in (!release!*.o) DO del %%f
for %%f in (!release!moc_*) DO del %%f
windeployqt release\Teststand.exe
for %%f in (!qtBin!\*lib*.dll) DO copy "%%f" !release!
