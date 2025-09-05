@echo off
setlocal enabledelayedexpansion

set DEPS=test pio

REM ===============================
REM Check if DEPS is empty
REM ===============================
if "%DEPS%"=="" (
    echo No dependencies provided. Exiting.
    goto :EOF
)

REM ===============================
REM Prepare README
REM ===============================
echo # Dependencies > README.md
echo. >> README.md
echo These dependencies are used by this projects tests/example codes. >> README.md
echo. >> README.md
echo All dependencies must fulfill these requirements: >> README.md
echo - C89 standard >> README.md
echo - nostdlib (No C Standard Library) >> README.md
echo - single header library >> README.md
echo - cross platform (Windows, Linux, MacOs) >> README.md
echo - open source license (MIT, Apache, ...) >> README.md
echo. >> README.md
echo --- >> README.md
echo. >> README.md
echo List of dependencies used: >> README.md

REM ===============================
REM Download dependencies and log info
REM ===============================
for %%D in (%DEPS%) do (
    wget -q -O %%D.h https://raw.githubusercontent.com/nickscha/%%D/refs/heads/main/%%D.h

    REM Get timestamp using %DATE% and %TIME%
    set "dt=%DATE%"
    set "tm=%TIME%"
    REM Format: YYYY-MM-DD HH:MM:SS
    set "TIMESTAMP=!dt:~6,4!-!dt:~3,2!-!dt:~0,2! !tm:~0,2!:!tm:~3,2!:!tm:~6,2!"

    REM Remove leading space in hour if present
    if "!TIMESTAMP:~11,1!"==" " set "TIMESTAMP=!TIMESTAMP:~0,11!0!TIMESTAMP:~12!"

    echo - %%D.h: https://github.com/nickscha/%%D - last updated: !TIMESTAMP! >> README.md
)

echo README.md generated successfully.
