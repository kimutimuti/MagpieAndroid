@echo off
echo ==========================================
echo   Magpie Android APK 构建
echo ==========================================
echo.

cd /d "%~dp0"

set ANDROID_HOME=%~dp0android-sdk

echo [1/2] 检查 Android SDK...
if not exist "%ANDROID_HOME%\cmdline-tools\latest\bin\sdkmanager.bat" (
    echo Android SDK 未正确安装！
    echo 请先运行 install-sdk.bat
    pause
    exit /b 1
)
echo SDK 已找到
echo.

echo [2/2] 开始构建 Debug APK（首次需要下载依赖，可能需要 5-10 分钟）...
echo.
call gradlew.bat assembleDebug --no-daemon

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ==========================================
    echo   ✓ 构建成功！
    echo ==========================================
    echo.
    echo APK 文件位置:
    echo %~dp0app\build\outputs\apk\debug\app-debug.apk
    echo.
    
    if exist "%~dp0app\build\outputs\apk\debug\app-debug.apk" (
        explorer.exe "%~dp0app\build\outputs\apk\debug"
    )
) else (
    echo.
    echo ==========================================
    echo   ✗ 构建失败！
    echo ==========================================
    echo.
    echo 退出代码: %ERRORLEVEL%
)

echo.
pause
