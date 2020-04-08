echo off
CALL:echored "IMPORTANT: REMOVE CYGWIN FROM PATH BEFORE INSTALLING."
:: Change to download directory
cd ..
SET ORIGINAL=%CD%
cd %UserProfile%\Downloads\

:: Download wget for later use
call:echoblue " ==================================== wget ===================================="
if not exist "%UserProfile%\Downloads\wget.exe" (
	echo wget not installed.
	if exist "%UserProfile%\Downloads\wget.exe" (
		echo wget-1.20.3 already downloaded.
	) else (
		echo Downloading wget for later use.
		bitsadmin.exe /transfer "wgetDownload" /priority Foreground http://eternallybored.org/misc/wget/1.20.3/64/wget.exe %UserProfile%\Downloads\wget.exe
	)
) else (
	echo wget already installed.
)

:: Download MinGW
call:echoblue " =================================== MingGW ==================================="
if not exist "%ProgramFiles%\mingw-w64\" (
	echo mingw-w64 not installed.
	if exist "%UserProfile%\Downloads\mingw-w64-install.exe" (
		echo mingw-w64-install.exe already downloaded.
	) else (
		echo Downloading MinGW.
		"%UserProfile%\Downloads\wget.exe" --no-check-certificate "https://sourceforge.net/projects/mingw-w64/files/Toolchains targetting Win32/Personal Builds/mingw-builds/installer/mingw-w64-install.exe" -O %UserProfile%\Downloads\mingw-w64-install.exe
	)
	echo Change Architecture to x86_64 if relevant.
	start /w %UserProfile%\Downloads\mingw-w64-install.exe
) else (
	echo MinGW already installed.
)

:: Download Cmake
call:echoblue " =================================== CMake ===================================="
if not exist "%ProgramFiles%\CMake\bin\" (
	echo CMake not installed.
	if exist "%UserProfile%\Downloads\cmake-3.7.1-win64-x64.msi" (
		echo cmake-3.7.1-win64-x64.msi already downloaded.
	) else (
		echo Downloading CMake.
		"%UserProfile%\Downloads\wget.exe" --no-check-certificate https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.msi
	)
	start /w %UserProfile%\Downloads\cmake-3.7.1-win64-x64.msi
) else (
	echo CMake already installed.
)

:: Download Git
call:echoblue " ==================================== Git ====================================="
if not exist "%ProgramFiles%\Git\cmd\" (
	echo Git not installed.
	if exist "%UserProfile%\Downloads\git-2.11.0-64-bit.exe" (
		echo cmake-3.7.1-win64-x64.msi already downloaded.
	) else (
		echo Downloading Git.
		"%UserProfile%\Downloads\wget.exe" --no-check-certificate https://github.com/git-for-windows/git/releases/download/v2.11.0.windows.1/Git-2.11.0-64-bit.exe
	)
	start /w %UserProfile%\Downloads\git-2.11.0-64-bit.exe
) else (
	echo Git already installed.
)

:: Download GnuMake for Windows
call:echoblue " =================================== GnuMake ===================================="
if not exist "%ProgramFiles(x86)%\GnuWin32\bin\" (
	echo GnuMake not installed.
	if exist "%UserProfile%\Downloads\make-3.81.exe" (
		echo make-3.81.exe already downloaded.
	) else (
		echo Downloading GnuMake.
		"%UserProfile%\Downloads\wget.exe" --no-check-certificate http://gnuwin32.sourceforge.net/downlinks/make.php
	)
	start /w %UserProfile%\Downloads\make-3.81.exe
) else (
	echo GnuMake already installed.
)

:: Add to path
call:ECHOGREEN "Add the following to your environmental variables."
echo %ProgramFiles(x86)%\GnuWin32\bin
echo %ProgramFiles%\mingw-w64\x86_64-6.3.0-posix-seh-rt_v5-rev0\mingw64\bin
echo %ProgramFiles%\CMake\bin
echo %ProgramFiles%\Git\cmd
call:ECHOGREEN "End of Vars."
rundll32 sysdm.cpl,EditEnvironmentVariables

:: Return to original directory
cd %ORIGINAL%\Installers
CALL:echored "IMPORTANT: Now open a NEW command prompt and run installDependiciesWindowsPart2.bat."

:: Functions
:ECHORED
%Windir%\System32\WindowsPowerShell\v1.0\Powershell.exe write-host -backgroundcolor Red %1
goto:eof

:ECHOBLUE
%Windir%\System32\WindowsPowerShell\v1.0\Powershell.exe write-host -backgroundcolor blue %1
goto:eof

:ECHOGREEN
%Windir%\System32\WindowsPowerShell\v1.0\Powershell.exe write-host -backgroundcolor green -foreground black %1
goto:eof