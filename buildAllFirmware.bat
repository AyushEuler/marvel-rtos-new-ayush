:: Deletes All files in the Build and Rebuilds 
::(Hidden, System, and Read-Only Files are Not Affected) 
::
@ECHO OFF 
rd /s /q Build
mkdir Build
cd .\Build\
cmake -G "MinGW Makefiles" --toolchain ../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug -DNDEBUG=1 .. 
@REM -NDEBUG = 1 ..
cmake --build . -- -j16
cd ..

:: static analyzer
::@REM set WORKSPACE_PATH=D:\BMS_PROJECT\marvel-rtos\marvel-rtos\
@REM set WORKSPACE_PATH=D:\marvel-rtos\
::@REM set WORKSPACE_PATH=D:\marvel-rtos\CMakeLists.txt
@REM set MAIN_PATH=%WORKSPACE_PATH%\Core\Src\
@REM cppcheck --platform=win64 --quiet --force --enable=information "%MAIN_PATH%\main.c" "%MAIN_PATH%stm32l4xx_it.c"
@REM set LIB_PATH=%WORKSPACE_PATH%Library\
@REM cppcheck --platform=win64 --quiet --force --enable=information "%LIB_PATH%Bootloader" "%LIB_PATH%Filter" 
@REM cppcheck --platform=win64 --quiet --force --enable=information "%LIB_PATH%Functional" "%LIB_PATH%Interface" 
@REM cppcheck --platform=win64 --quiet --force --enable=information "%LIB_PATH%Math"       "%LIB_PATH%Memory"    
@REM cppcheck --platform=win64 --quiet --force --enable=information "%LIB_PATH%Peripheral" "%LIB_PATH%Threads" "%LIB_PATH%Utility"