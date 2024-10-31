@ECHO OFF

echo "copy: %~dp0..\..\..\projects\msvc\libshapefile\output\%1\libshapefile.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\projects\msvc\libshapefile\output\%1\libshapefile.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\projects\msvc\libgeodbapi\output\%1\libgeodbapi.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\projects\msvc\libgeodbapi\output\%1\libgeodbapi.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadVC2.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadVC2.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadGC2.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadGC2.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\cairo-windows-1.17.2\lib\x64\cairo.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\cairo-windows-1.17.2\lib\x64\cairo.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\SDL2-2.26.5\lib\x64\SDL2.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\SDL2-2.26.5\lib\x64\SDL2.dll" "%~dp0output\%1\"

copy /Y "%~dp0..\..\..\earth.bmp" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\maplayers.cfg => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\maplayers.cfg" "%~dp0output\%1\"

if "%1" == "x64-Debug" (
	echo "update %1 DLLs OK"
)

if "%1" == "x64-Release" (
	echo "copy: %~dp0..\..\..\deps\mingw64-bin\*.dll => %~dp0output\%1\"
    copy /Y "%~dp0..\..\..\deps\mingw64-bin\*.dll" "%~dp0output\%1\"

    echo "copy: %~dp0..\..\..\deps\VC_redist.x64.exe => %~dp0output\%1\"
    copy /Y "%~dp0..\..\..\deps\VC_redist.x64.exe" "%~dp0output\%1\"

	echo "update %1 DLLs OK"
)
