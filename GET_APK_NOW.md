# 🚀 一键获取 Magpie Android APK（最简单方法）

## 5 分钟拿到 APK！

### 第一步：创建 GitHub 仓库（2 分钟）

1. 访问 https://github.com/new
2. 填写：
   - Repository name: `MagpieAndroid`
   - 选择 **Public**（免费）
   - 点击 **Create repository**

### 第二步：上传代码（2 分钟）

在你的电脑上打开 PowerShell，依次运行：

```powershell
cd e:\工作\MagpieAndroid

git init
git add .
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/你的用户名/MagpieAndroid.git
git push -u origin main
```

**注意：把上面的 "你的用户名" 换成你实际的 GitHub 用户名！**

### 第三步：下载 APK（1 分钟）

1. 打开你的 GitHub 仓库页面
2. 点击顶部的 **Actions** 标签
3. 等待 5-10 分钟，构建自动完成
4. 点击最新的一次运行记录
5. 滚动到页面底部的 **Artifacts**
6. 下载 `app-debug`
7. 解压后就是 APK 文件！

---

## APK 安装到手机

1. 将 `app-debug.apk` 传到手机
2. 在手机设置中开启"允许安装未知来源应用"
3. 点击 APK 文件安装

---

## 已有的文件说明

| 文件 | 说明 |
|------|------|
| `QUICK_START.md` | 详细使用指南 |
| `BUILD_GUIDE.md` | 本地构建指南 |
| `build-simple.bat` | 本地构建脚本 |
| `.github/workflows/build.yml` | GitHub Actions 自动构建配置 |

---

## 项目特点

✅ **4 种超分算法**：Anime4K、FSR、Bicubic、Lanczos  
✅ **缩放可调**：1.5x - 3.0x  
✅ **无需 Root**：使用 MediaProjection API  
✅ **全屏超分**：悬浮窗显示  

---

有问题？查看 `QUICK_START.md` 获取更多帮助！
