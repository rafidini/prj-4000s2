rem set up the msvc 64bit compiler

set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\amd64
set PATH=%PATH%;C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\x64
rem set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE

set INCLUDE=%INCLUDE%;C:\Program Files\Microsoft SDKs\Windows\v7.1\Include
set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include

set LIB=%LIB%;C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\x64
set LIB=%LIB%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\amd64

rem JOM
set PATH=%PATH%;C:\Program Files\jom_1_0_11

rem Python 2.2
set PATH=%PATH%;D:\Build\Python22_32
set INCLUDE=%INCLUDE%;D:\Build\boost_1_49_0\boost_python22\boost_1_49_0
set LIB=%LIB%;D:\Build\boost_1_49_0\boost_python22\boost_1_49_0\stage\lib
set LIB=%LIB%;D:\Build\Python22_32\libs

cmake -G "NMake Makefiles JOM" -C C:\Users\ujoimro\Pink\pink-hg\data\configs\pink_windows_config.txt C:\Users\ujoimro\Pink\pink-hg
cmake -G "NMake Makefiles JOM" -C C:\Users\ujoimro\Pink\pink-hg\data\configs\pink_windows_config.txt C:\Users\ujoimro\Pink\pink-hg
cmake -G "NMake Makefiles JOM" -C C:\Users\ujoimro\Pink\pink-hg\data\configs\pink_windows_config.txt C:\Users\ujoimro\Pink\pink-hg

rem nmake
jom -j4
jom -j4 source_package
jom -j4 package