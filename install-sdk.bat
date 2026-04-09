@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo   Magpie Android SDK 组件安装
echo ==========================================
echo.

set ANDROID_HOME=%~dp0android-sdk
set PATH=%ANDROID_HOME%\cmdline-tools\latest\bin;%PATH%

echo [1/4] 安装 platform-tools...
echo y| call "%ANDROID_HOME%\cmdline-tools\latest\bin\sdkmanager.bat" "platform-tools"
if errorlevel 1 (
    echo 安装失败！
    exit /b 1
)
echo 完成！
echo.

echo [2/4] 安装 Android 34 平台...
echo y| call "%ANDROID_HOME%\cmdline-tools\latest\bin\sdkmanager.bat" "platforms;android-34"
if errorlevel 1 (
    echo 安装失败！
    exit /b 1
)
echo 完成！
echo.

echo [3/4] 安装 Build Tools 34.0.0...
echo y| call "%ANDROID_HOME%\cmdline-tools\latest\bin\sdkmanager.bat" "build-tools;34.0.0"
if errorlevel 1 (
    echo 安装失败！
    exit /b 1
)
echo 完成！
echo.

echo [4/4] 安装 CMake 3.22.1...
echo y| call "%ANDROID_HOME%\cmdline-tools\latest\bin\sdkmanager.bat" "cmake;3.22.1"
if errorlevel 1 (
    echo 安装失败！
    exit /b 1
)
echo 完成！
echo.

echo ==========================================
echo   所有组件安装完成！
echo ==========================================
