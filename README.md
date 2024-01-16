# marvel-rtos
# MARVEL3
Repo with Firmware for MARVEL3 BMS.
# Prerequisites
 1. Insall ARM GCC Toolchain
 2. openOCD (version : 0.11.0-2-win32-x64)
 3. Cmake./
 4. Make
# Building
For Windows:
    `./buildAllFirmware.bat`
For Linux
 ` mkdir build && cd build `
 Generate cmake files
 `cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ..`
 For windows
 `cmake -G "Unix Makefiles" --toolchain ../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ..`
 Building
 `cmake --build . -- -j12`
# Flashing
For windows
    `./flashAllFirmware.bat`
For Linux
    :TODO
FLAG Partition
FLAG | BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7
--- | --- | --- | --- | --- |--- |--- |--- |---
USE | CHANGE PARTITION |
FLAG | BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |
FLAG | BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |  |
FLAG | BIT24 | BIT25 | BIT26 | BIT27 | BIT28 | BIT29 | BIT30 | BIT31
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |
#versioning
Firmware Version : Example 22081 corresponds 2022, August 1st Release.
Marvel Config    : 4X - 4Wheeler - Config Number "X".
                   42 - Base from Firmware.
                   43 - Threshold Retries Logic Added.
