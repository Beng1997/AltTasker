@echo off
REM AltTasker - Quick Run in WSL
REM This batch file makes it easy to run AltTasker from Windows

echo.
echo ========================================
echo   AltTasker - Linux Task Monitor
echo ========================================
echo.

REM Check if WSL is available
wsl --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: WSL is not installed or not available
    echo Please install WSL first: https://aka.ms/wsl
    pause
    exit /b 1
)

echo Starting AltTasker in WSL...
echo.
echo TIP: Press Ctrl+C to exit the application
echo.
timeout /t 2 /nobreak >nul

REM Run the script in WSL
wsl bash /mnt/d/AltTasker/scripts/run-in-wsl.sh

echo.
echo AltTasker has been closed.
pause
