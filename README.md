# Magpie Android

Android 版本的超分辨率应用，基于 Magpie 项目的思想。

## 功能特性

- 支持多种超分辨率算法：
  - Anime4K（适合动漫和游戏）
  - FSR（AMD FidelityFX Super Resolution）
  - Bicubic（双三次插值）
  - Lanczos（Lanczos 插值）
- 可调节缩放比例（1.5x - 3.0x）
- 全屏超分显示
- 无需 Root 权限（使用 MediaProjection API）

## 系统要求

- Android 7.0 (API 24) 或更高版本
- 支持 OpenGL ES 3.0 的设备

## 使用说明

1. 安装应用并授予屏幕录制权限
2. 选择超分效果
3. 调整缩放比例
4. 点击"开始超分"按钮
5. 应用会在屏幕上显示超分后的画面

## 编译说明

### 前置要求

- Android Studio Hedgehog (2023.1.1) 或更高版本
- Android NDK 25.2.9519653 或更高版本
- CMake 3.22.1 或更高版本

### 编译步骤

1. 克隆或下载项目
2. 使用 Android Studio 打开项目
3. 等待 Gradle 同步完成
4. 连接 Android 设备或启动模拟器
5. 点击 Run 按钮编译并运行应用

## 技术架构

### 核心组件

- **MediaProjection API**: 用于屏幕捕获
- **OpenGL ES 3.0**: 用于 GPU 加速的超分渲染
- **NDK/JNI**: 用于 C++ 渲染引擎
- **Foreground Service**: 用于后台持续运行

### 项目结构

```
app/
├── src/main/
│   ├── cpp/                    # C++ 代码
│   │   ├── effects/            # 超分效果着色器
│   │   ├── Renderer.cpp        # 渲染器实现
│   │   └── native-lib.cpp      # JNI 绑定
│   ├── java/com/magpie/android/
│   │   ├── MainActivity.kt     # 主界面
│   │   ├── ScreenCaptureService.kt  # 屏幕捕获服务
│   │   └── MagpieGLView.kt     # OpenGL 视图
│   └── res/                    # 资源文件
```

## 注意事项

1. 应用需要悬浮窗权限才能显示超分画面
2. 性能取决于设备 GPU 能力
3. 较高的缩放比例可能导致帧率下降
4. 某些应用可能会检测到屏幕录制并阻止内容显示

## 许可证

本项目遵循 Magpie 原项目的许可证。
