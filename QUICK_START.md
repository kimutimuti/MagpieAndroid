# Magpie Android - 快速获取 APK 指南

## 🚀 最快方法：使用 GitHub Actions 自动构建

### 步骤 1：创建 GitHub 仓库

1. 访问 [GitHub](https://github.com) 并登录
2. 点击 "New repository" 创建新仓库
3. 命名为 `MagpieAndroid`（或其他你喜欢的名字）
4. 选择 "Public" 或 "Private"
5. 点击 "Create repository"

### 步骤 2：上传代码

在你的电脑上打开 PowerShell 或命令提示符：

```powershell
cd e:\工作\MagpieAndroid

# 初始化 Git 仓库
git init

# 添加所有文件
git add .

# 提交
git commit -m "Initial commit"

# 关联到你的 GitHub 仓库（替换下面的用户名）
git remote add origin https://github.com/你的用户名/MagpieAndroid.git

# 推送代码
git branch -M main
git push -u origin main
```

### 步骤 3：自动构建 APK

1. 打开你的 GitHub 仓库页面
2. 点击 "Actions" 标签
3. 你会看到 "Build Android APK" 工作流正在运行
4. 等待约 5-10 分钟，构建完成后
5. 点击最新的工作流运行记录
6. 在页面底部的 "Artifacts" 部分，你会看到：
   - `app-debug.apk` - 调试版本（可以直接安装）
   - `app-release-unsigned.apk` - 发布版本

### 步骤 4：下载 APK

1. 点击 `app-debug` 下载
2. 解压下载的文件
3. 将 APK 传输到你的 Android 设备
4. 在设备上允许安装未知来源应用
5. 点击安装！

---

## 📱 使用 Android Studio 构建（备选方案）

如果你想在本地构建：

### 1. 安装 Android Studio

下载地址：https://developer.android.com/studio

### 2. 打开项目

1. 启动 Android Studio
2. 选择 "Open an Existing Project"
3. 选择 `e:\工作\MagpieAndroid` 文件夹
4. 等待 Gradle 同步（首次可能需要 5-10 分钟）

### 3. 构建 APK

菜单：Build → Build Bundle(s) / APK(s) → Build APK(s)

APK 位置：`app\build\outputs\apk\debug\app-debug.apk`

---

## 🎯 应用功能

- ✅ Anime4K 超分（适合动漫/游戏）
- ✅ FSR 超分
- ✅ Bicubic 插值
- ✅ Lanczos 插值
- ✅ 1.5x - 3.0x 缩放可调
- ✅ 无需 Root 权限
- ✅ 全屏超分显示

---

## ⚙️ 使用说明

1. 打开应用
2. 授予屏幕录制权限
3. 授予悬浮窗权限
4. 选择超分效果
5. 调整缩放比例
6. 点击"开始超分"

---

## ❓ 常见问题

**Q: GitHub Actions 构建失败怎么办？**
A: 检查 Actions 日志中的错误信息，通常是依赖下载问题，重新运行工作流即可。

**Q: 安装时提示"解析包错误"？**
A: 确保你的 Android 版本是 7.0 (API 24) 或更高。

**Q: 应用无法启动？**
A: 确保授予了所有必要的权限（屏幕录制、悬浮窗）。

---

祝你使用愉快！🎮
