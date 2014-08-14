rem if not exist "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\lib" (
rem md "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\lib")

rem if not exist "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\inc" (
rem md "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\inc")

rem copy ".\Debug\Exe\AfLib_F.lib" "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\lib\"

rem copy ".\AfSrc\*.h"             "D:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\8051\AfLib_F\inc\"

set AFLIB_PATH="%IAR_EW8051_PATH%\AfLib_F\lib"
set AFINC_PATH="%IAR_EW8051_PATH%\AfLib_F\inc"

if not exist %AFLIB_PATH% md %AFLIB_PATH%
if not exist %AFINC_PATH% md %AFINC_PATH%

copy ".\Debug\Exe\AfLib_F.lib" %AFLIB_PATH%
copy ".\AfSrc\*.h"             %AFINC_PATH%