# Magpie Android 构建指南

## 前置要求

1. **Java Development Kit (JDK) 17** - 已安装 ✓
2. **Android Studio** (推荐) 或 **Android Command Line Tools**
3. **Android NDK** (版本 25.2.9519653 或更高)
4. **CMake** (3.22.1 或更高)

## 方法一：使用 Android Studio 构建（推荐）

### 步骤 1: 安装 Android Studio
1. 下载并安装 [Android Studio](https://developer.android.com/studio)
2. 启动 Android Studio，完成初始设置

### 步骤 2: 安装必要的 SDK 组件
1. 打开 Android Studio
2. 进入 Tools → SDK Manager
3. 在 "SDK Platforms" 标签页中，勾选：
   - Android 13.0 (API 33) 或更高版本
4. 在 "SDK Tools" 标签页中，勾选：
   - Android SDK Build-Tools 34.0.0
   - NDK (Side by side) 25.2.9519653
   - CMake 3.22.1
   - Android SDK Platform-Tools
5. 点击 "Apply" 安装

### 步骤 3: 打开项目
1. 在 Android Studio 中选择 "Open an Existing Project"
2. 选择 `e:\工作\MagpieAndroid` 文件夹
3. 等待 Gradle 同步完成（首次可能需要几分钟）

### 步骤 4: 构建 APK
1. 连接 Android 设备或启动模拟器（可选）
2. 菜单选择 Build → Build Bundle(s) / APK(s) → Build APK(s)
3. 等待构建完成
4. APK 文件位置：
   - Debug: `app\build\outputs\apk\debug\app-debug.apk`
   - Release: `app\build\outputs\apk\release\app-release.apk`

## 方法二：使用命令行构建

### 设置环境变量

首先需要设置以下环境变量：

```powershell
# 设置 Android SDK 路径（根据你的实际安装位置修改）
$env:ANDROID_HOME = "C:\Users\你的用户名\AppData\Local\Android\Sdk"
$env:ANDROID_NDK_HOME = "$env:ANDROID_HOME\ndk\25.2.9519653"
$env:PATH += ";$env:ANDROID_HOME\platform-tools;$env:ANDROID_HOME\cmdline-tools\latest\bin"
```

### 构建命令

```powershell
cd e:\工作\MagpieAndroid

# 首次运行需要下载 Gradle
.\gradlew.bat wrapper

# 构建 Debug APK
.\gradlew.bat assembleDebug

# 构建 Release APK
.\gradlew.bat assembleRelease

# 清理构建
.\gradlew.bat clean
```

## 方法三：使用在线构建服务（最简单）

如果你不想安装 Android SDK，可以使用以下在线服务：

1. **GitHub Actions** - 创建 GitHub 仓库，推送代码，使用 Actions 自动构建
2. **AppVeyor** - 免费的 CI/CD 服务
3. **CircleCI** - 另一个流行的 CI/CD 平台

## 项目结构说明

```
MagpieAndroid/
├── app/
│   ├── src/main/
│   │   ├── cpp/              # C++ 渲染引擎
│   │   │   ├── effects/      # 超分效果着色器
│   │   │   └── Renderer.cpp
│   │   ├── java/             
│   │   │   └── com/magpie/android/
│   │   │       ├── MainActivity.kt
│   │   │       ├── ScreenCaptureService.kt
│   │   │       └── MagpieGLView.kt
│   │   └── res/              # 资源文件
│   └── build.gradle.kts
├── gradle/
├── build.gradle.kts
└── settings.gradle.kts
```

## 常见问题

### Q: Gradle 同步失败
A: 检查网络连接，Gradle 需要下载依赖。可以配置国内镜像源。

### Q: NDK 找不到
A: 确保在 `local.properties` 文件中设置了正确的 NDK 路径，或在项目结构中配置。

### Q: 构建很慢
A: 首次构建需要下载很多依赖，后续会快很多。

### Q: 如何安装到设备
A: 使用 `adb install app-debug.apk` 命令，或在 Android Studio 中点击 Run 按钮。

## 下一步

构建成功后，你可以：
1. 优化超分算法
2. 添加更多效果
3. 改进 UI/UX
4. 优化性能

祝你构建顺利！
