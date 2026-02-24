# clumsy

<p align="center">
  <img src="https://img.shields.io/badge/platform-Windows-blue" alt="Platform">
  <img src="https://img.shields.io/badge/license-MIT-green" alt="License">
  <img src="https://img.shields.io/badge/Zig-0.9.1-orange" alt="Zig Version">
</p>

---

### 🇨🇳 中文版 | [Read in English](./README.zh-CN.md)

---

__clumsy makes your network condition on Windows significantly worse, but in a managed and interactive manner.__

Leveraging the awesome [WinDivert](http://reqrypt.org/windivert.html), clumsy stops living network packets and capture them, lag/drop/tamper/.. the packets on demand, then send them away. Whether you want to track down weird bugs related to broken network, or evaluate your application on poor connections, clumsy will come in handy:

* No installation.
* No need for proxy setup or code change in your application.
* System wide network capturing means it works on any application.
* Works even if you're offline (ie, connecting from localhost to localhost).
* Your application keeps running, while clumsy can start and stop anytime.
* Interactive control how bad the network can be, with enough visual feedback to tell you what's going on.

See [this page](http://jagt.github.io/clumsy) for more info and build instructions.

---

## ✨ New Features

### 🌐 Multi-language Support

clumsy now supports multiple languages! You can switch between:

| Language | Description |
|---------|-------------|
| 🇺🇸 English | Default language |
| 🇨🇳 简体中文 | Simplified Chinese |
| 🇭🇰 繁體中文 | Traditional Chinese |
| 🇯🇵 日本語 | Japanese |

**How to Change Language:**
1. Click the **Language** dropdown menu in the toolbar
2. Select your preferred language
3. The interface will update immediately

> 💡 Tip: Your language preference is saved automatically.

---

### 🚀 Cloud Builds with GitHub Actions

The project now supports automated cloud builds via GitHub Actions!

- **Automatic builds** on every push to `main`/`master` branch
- **Multi-architecture support**: x86 and x64
- **Release automation**: Creates downloadable artifacts for each release

| Artifact | Description |
|----------|-------------|
| `clumsy-x86-Release.zip` | 32-bit Release build |
| `clumsy-x64-Release.zip` | 64-bit Release build |

---

## 📥 Download

Get the latest release from: [https://github.com/August-GitHub/Clumsy-CN/releases](https://github.com/August-GitHub/Clumsy-CN/releases)

---

## 🛠️ Build from Source

### Prerequisites

- [Zig](https://ziglang.org/) 0.9.1
- [WinDivert](https://reqrypt.org/windivert.html) 2.2.0
- Windows SDK (for resource compilation)

### Build Commands

```bash
# Build for x86 (32-bit)
zig build -Darch=x86 -Dconf=Release

# Build for x64 (64-bit)
zig build -Darch=x64 -Dconf=Release
```

---

## 📸 Screenshots

Simulate network latency, delay, packet loss with clumsy on Windows 7/8/10:

![](clumsy-demo.gif)

---

## 📝 License

MIT

---

## 🙏 Acknowledgments

- [WinDivert](http://reqrypt.org/windivert.html) - Windows Packet Divert Library
- [IUP](https://www.tecgraf.puc-rio.br/iup/) - Portable User Interface Library
- Original [clumsy](https://github.com/jagt/clumsy) project
