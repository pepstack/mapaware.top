@ECHO OFF

echo "copy: %~dp0..\..\..\..\shapefile\projects\msvc\shapefile\output\%1\shapefile.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\..\shapefile\projects\msvc\shapefile\output\%1\shapefile.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadVC2.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadVC2.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadGC2.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\pthreads-w32\Pre-built.2\dll\x64\pthreadGC2.dll" "%~dp0output\%1\"

echo "copy: %~dp0..\..\..\deps\cairo-windows-1.17.2\lib\x64\cairo.dll => %~dp0output\%1\"
copy /Y "%~dp0..\..\..\deps\cairo-windows-1.17.2\lib\x64\cairo.dll" "%~dp0output\%1\"

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
