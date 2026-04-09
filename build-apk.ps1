# Magpie Android 自动构建脚本
# 此脚本会自动下载 Android SDK、NDK 并构建 APK

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  Magpie Android APK 自动构建工具" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

$ErrorActionPreference = "Stop"

# 设置目录
$workDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$androidSdkDir = "$workDir\android-sdk"
$cmdlineToolsDir = "$androidSdkDir\cmdline-tools\latest"
$ndkVersion = "25.2.9519653"
$ndkDir = "$androidSdkDir\ndk\$ndkVersion"

Write-Host "工作目录: $workDir" -ForegroundColor Yellow
Write-Host "Android SDK 目录: $androidSdkDir" -ForegroundColor Yellow
Write-Host ""

# 创建目录
if (-not (Test-Path $androidSdkDir)) {
    New-Item -ItemType Directory -Path $androidSdkDir -Force | Out-Null
}

# 检查是否已下载 SDK
if (-not (Test-Path "$cmdlineToolsDir\bin\sdkmanager.bat")) {
    Write-Host "[1/6] 下载 Android Command Line Tools..." -ForegroundColor Green
    
    $sdkUrl = "https://dl.google.com/android/repository/commandlinetools-win-9477386_latest.zip"
    $sdkZip = "$workDir\commandlinetools.zip"
    
    Write-Host "从 $sdkUrl 下载..." -ForegroundColor Gray
    Invoke-WebRequest -Uri $sdkUrl -OutFile $sdkZip -UseBasicParsing
    
    Write-Host "解压中..." -ForegroundColor Gray
    Expand-Archive -Path $sdkZip -DestinationPath "$androidSdkDir\temp" -Force
    
    # 移动到正确位置
    New-Item -ItemType Directory -Path "$androidSdkDir\cmdline-tools" -Force | Out-Null
    Move-Item -Path "$androidSdkDir\temp\cmdline-tools" -Destination "$cmdlineToolsDir" -Force
    
    Remove-Item -Path $sdkZip -Force
    Remove-Item -Path "$androidSdkDir\temp" -Recurse -Force
    
    Write-Host "✓ Android Command Line Tools 安装完成" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host "[1/6] Android Command Line Tools 已存在，跳过下载" -ForegroundColor Green
    Write-Host ""
}

# 设置环境变量
$env:ANDROID_HOME = $androidSdkDir
$env:ANDROID_SDK_ROOT = $androidSdkDir
$env:PATH = "$cmdlineToolsDir\bin;$androidSdkDir\platform-tools;$env:PATH"

# 创建 local.properties
Write-Host "[2/6] 配置项目..." -ForegroundColor Green
$localProps = @"
sdk.dir=$($androidSdkDir -replace '\\', '\\\\')
ndk.dir=$($ndkDir -replace '\\', '\\\\')
"@
Set-Content -Path "$workDir\local.properties" -Value $localProps -Encoding UTF8
Write-Host "✓ local.properties 已创建" -ForegroundColor Green
Write-Host ""

# 接受许可证
Write-Host "[3/6] 接受 Android SDK 许可证..." -ForegroundColor Green
$yesPath = "$env:TEMP\yes.txt"
"y`n" * 20 | Out-File -FilePath $yesPath -Encoding ASCII
Get-Content $yesPath | &amp; "$cmdlineToolsDir\bin\sdkmanager.bat" --licenses | Out-Null
Remove-Item -Path $yesPath -Force -ErrorAction SilentlyContinue
Write-Host "✓ 许可证已接受" -ForegroundColor Green
Write-Host ""

# 安装必要的 SDK 组件
Write-Host "[4/6] 安装 Android SDK 组件（这可能需要几分钟）..." -ForegroundColor Green
$packages = @(
    "platform-tools",
    "platforms;android-34",
    "build-tools;34.0.0",
    "ndk;$ndkVersion",
    "cmake;3.22.1"
)

foreach ($pkg in $packages) {
    Write-Host "安装: $pkg" -ForegroundColor Gray
    &amp; "$cmdlineToolsDir\bin\sdkmanager.bat" "$pkg" | Out-Null
    Write-Host "  ✓ 完成" -ForegroundColor Gray
}
Write-Host "✓ 所有 SDK 组件安装完成" -ForegroundColor Green
Write-Host ""

# 构建 APK
Write-Host "[5/6] 开始构建 APK..." -ForegroundColor Green
Set-Location -Path $workDir

# 首次运行需要设置 gradlew 权限
if (-not $IsWindows) {
    chmod +x gradlew
}

Write-Host "执行 Gradle 构建..." -ForegroundColor Gray
if ($IsWindows) {
    &amp; .\gradlew.bat assembleDebug
} else {
    &amp; ./gradlew assembleDebug
}

if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ 构建成功！" -ForegroundColor Green
    Write-Host ""
    
    $apkPath = "$workDir\app\build\outputs\apk\debug\app-debug.apk"
    if (Test-Path $apkPath) {
        Write-Host "[6/6] APK 文件位置:" -ForegroundColor Cyan
        Write-Host $apkPath -ForegroundColor Yellow
        Write-Host ""
        Write-Host "==========================================" -ForegroundColor Cyan
        Write-Host "  ✓ APK 构建完成！" -ForegroundColor Green
        Write-Host "==========================================" -ForegroundColor Cyan
        Write-Host ""
        
        # 尝试打开 APK 所在目录
        explorer.exe (Split-Path -Parent $apkPath)
    } else {
        Write-Host "✗ APK 文件未找到！" -ForegroundColor Red
    }
} else {
    Write-Host "✗ 构建失败！退出代码: $LASTEXITCODE" -ForegroundColor Red
    exit 1
}
