# Ping-Status

一款现代化的命令行 HTTP 请求分析工具，以美观的可主题化界面显示详细的时序信息和状态码。

For English documentation, please see [README.md](README.md).

## 功能特点

- **详细的 HTTP 时序分析**：测量并显示 DNS 查询、TCP 连接、TLS 握手、服务器处理和内容传输时间
- **美观的可主题化界面**：受现代系统监视器（如 btop）启发的多种颜色主题
- **实时可视化**：进度条和历史图表用于跟踪性能变化
- **JSON 输出支持**：机器可读的输出格式，便于脚本和自动化
- **连续监控**：可配置间隔时间持续监控端点
- **可定制配置**：广泛的外观和行为配置选项

## 安装

### 先决条件

- libcurl 开发库
- ncurses 开发库
- json-c 开发库

#### Ubuntu/Debian:
```bash
sudo apt-get install libcurl4-openssl-dev libncursesw5-dev libjson-c-dev
```

# On CentOS/RHEL:
```bash
sudo yum install libcurl-devel ncurses-devel json-c-devel
```

# On Arch Linux:
```bash
sudo pacman -S base-devel ncurses cjson
```

# Gentoo Linux:
```bash
sudo emerge dev-libs/ncurses dev-libs/cjson
```

# macOS with Homebrew:
```terminal
brew install ncurses cjson
```

### 从源码编译
```bash
git clone https://github.com/HTML0727/ping-status.git
cd ping-status
make
sudo make install
```

这将把 ping-status 二进制文件安装到 ```/usr/local/bin```，并将默认配置文件安装到 ```/usr/local/share/ping-status/```。

## 配置

**Ping-Status** 使用位于 ```~/.config/ping-status/``` 的配置文件。首次运行时，如果该目录不存在，将创建该目录并包含默认配置文件。

### 配置文件
```~/.config/ping-status/config.conf```

应用程序行为的主要配置文件：

```ini
# Ping-Status 配置文件
# 此文件控制 ping-status 应用程序的行为

[general]
# 未提供 URL 时的默认检查地址
# default_url = https://example.com
# 默认检查间隔（秒，用于连续模式）
interval = 5
# 默认请求次数
count = 1
# 默认输出格式 (text, json)
output_format = text
# 默认主题 (tokyo-night, adiwata, apple-system, custom)
theme = tokyo-night
# 是否验证 SSL 证书 (0 = 否, 1 = 是)
verify_ssl = 1
# HTTP 请求超时时间（秒）
timeout = 10
# 用于请求的 User-Agent 字符串
user_agent = ping-status/1.0

[display]
# UI 刷新率（毫秒）
refresh_rate = 100
# 是否显示进度条
show_progress_bars = 1
# 是否显示历史图表
show_history = 1
# 保留的历史记录最大条目数
max_history_entries = 50
# 是否显示详细时序信息
show_detailed_timing = 1

[behavior]
# 是否跟随重定向
follow_redirects = 1
# 最大重定向次数
max_redirects = 10
# 是否在计时中包含响应体 (0 = 仅头部, 1 = 完整响应)
include_body = 1
# 是否解析 DNS 用于显示
resolve_dns = 1
```

```~/.config/ping-status/css.conf```

基于 CSS 的主题配置：

```css
/* Ping-Status 主题配置 */
/* 此文件控制 ping-status 的视觉外观 */

:root {
  /* 基础颜色 - 所有主题使用，除非被覆盖 */
  --primary-color: #1a1b26;
  --secondary-color: #24283b;
  --accent-color: #bb9af7;
  --background-color: #16161e;
  --text-color: #a9b1d6;
  --success-color: #73daca;
  --warning-color: #e0af68;
  --error-color: #f7768e;
  --border-color: #414868;
  --highlight-color: #7aa2f7;
}

/* Tokyo Night 主题 (默认) */
.tokyo-night {
  --primary-color: #1a1b26;
  --secondary-color: #24283b;
  --accent-color: #bb9af7;
  --background-color: #16161e;
  --text-color: #a9b1d6;
  --success-color: #73daca;
  --warning-color: #e0af68;
  --error-color: #f7768e;
  --border-color: #414868;
  --highlight-color: #7aa2f7;
}

/* Adiwata 主题 */
.adiwata {
  --primary-color: #2a2b36;
  --secondary-color: #343746;
  --accent-color: #6d8cbd;
  --background-color: #22232e;
  --text-color: #c5c8d1;
  --success-color: #8dc776;
  --warning-color: #e6b455;
  --error-color: #e35555;
  --border-color: #44475a;
  --highlight-color: #5e81ac;
}

/* Apple System 主题 */
.apple-system {
  --primary-color: #ffffff;
  --secondary-color: #f5f5f7;
  --accent-color: #007aff;
  --background-color: #ffffff;
  --text-color: #333333;
  --success-color: #34c759;
  --warning-color: #ff9500;
  --error-color: #ff3b30;
  --border-color: #c6c6c8;
  --highlight-color: #007aff;
}

/* 自定义主题 - 取消注释并修改以创建自己的主题 */
/*
.custom-theme {
  --primary-color: #your-color;
  --secondary-color: #your-color;
  --accent-color: #your-color;
  --background-color: #your-color;
  --text-color: #your-color;
  --success-color: #your-color;
  --warning-color: #your-color;
  --error-color: #your-color;
  --border-color: #your-color;
  --highlight-color: #your-color;
}
*/

/* UI 元素 */
.status-container {
  background-color: var(--background-color);
  color: var(--text-color);
  border: 1px solid var(--border-color);
  border-radius: 8px;
  padding: 1rem;
  margin: 0.5rem;
}

.status-header {
  color: var(--accent-color);
  border-bottom: 1px solid var(--border-color);
  padding-bottom: 0.5rem;
  margin-bottom: 1rem;
  font-weight: bold;
}

.timing-item {
  display: flex;
  justify-content: space-between;
  padding: 0.25rem 0;
  border-bottom: 1px solid var(--secondary-color);
}

.timing-value {
  font-weight: bold;
}

.timing-value.good {
  color: var(--success-color);
}

.timing-value.warning {
  color: var(--warning-color);
}

.timing-value.bad {
  color: var(--error-color);
}

.progress-bar {
  height: 8px;
  background-color: var(--secondary-color);
  border-radius: 4px;
  overflow: hidden;
  margin-top: 0.25rem;
}

.progress-fill {
  height: 100%;
  background-color: var(--accent-color);
  transition: width 0.3s ease;
}

.progress-fill.good {
  background-color: var(--success-color);
}

.progress-fill.warning {
  background-color: var(--warning-color);
}

.progress-fill.bad {
  background-color: var(--error-color);
}

.http-status {
  display: inline-block;
  padding: 0.25rem 0.5rem;
  border-radius: 4px;
  font-weight: bold;
}

.http-status-2xx {
  background-color: var(--success-color);
  color: var(--background-color);
}

.http-status-3xx {
  background-color: var(--warning-color);
  color: var(--background-color);
}

.http-status-4xx, .http-status-5xx {
  background-color: var(--error-color);
  color: var(--background-color);
}

.history-chart {
  height: 60px;
  background-color: var(--secondary-color);
  border-radius: 4px;
  display: flex;
  align-items: flex-end;
  padding: 0.25rem;
  gap: 2px;
}

.history-bar {
  background-color: var(--accent-color);
  border-radius: 2px 2px 0 0;
  flex: 1;
  min-width: 2px;
}

.history-bar.good {
  background-color: var(--success-color);
}

.history-bar.warning {
  background-color: var(--warning-color);
}

.history-bar.bad {
  background-color: var(--error-color);
}

.control-button {
  background-color: var(--secondary-color);
  border: 1px solid var(--border-color);
  color: var(--text-color);
  padding: 0.5rem 1rem;
  border-radius: 4px;
  cursor: pointer;
  margin-right: 0.5rem;
}

.control-button:hover {
  background-color: var(--accent-color);
  color: var(--background-color);
}

.theme-button {
  width: 20px;
  height: 20px;
  border-radius: 50%;
  border: 2px solid var(--border-color);
  cursor: pointer;
  display: inline-block;
  margin-right: 0.5rem;
}

.theme-button.active {
  border-color: var(--accent-color);
}

.theme-button.tokyo-night {
  background-color: #bb9af7;
}

.theme-button.adiwata {
  background-color: #6d8cbd;
}

.theme-button.apple-system {
  background-color: #007aff;
}

.loading {
  opacity: 0.7;
  pointer-events: none;
}

.loading::after {
  content: '';
  display: inline-block;
  width: 12px;
  height: 12px;
  border: 2px solid transparent;
  border-top: 2px solid var(--accent-color);
  border-radius: 50%;
  animation: spin 1s linear infinite;
  margin-left: 8px;
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}
```

## 使用方法

### 基本用法

```bash
ping-status https://example.com
```

### 使用选项

```bash
# 自定义请求间隔
ping-status -i 10 https://example.com
# 连续监控
ping-status -C https://example.com
# JSON 输出格式
ping-status -o json https://example.com
# 自定义主题
ping-status -t adiwata https://example.com
# 多个请求
ping-status -c 5 https://example.com
```

### 命令行选项
- ```-i, --interval SECONDS``` : 请求间隔（默认：5）
- ```-c, --count NUMBER``` : 请求次数（默认：1）
- ```-C, --continuous``` : 连续模式（直到被中断）
- ```-v, --verbose``` :  详细输出
- ```-t, --theme THEME``` : 颜色主题 (tokyo-night, adiwata, apple-system, custom)
- ```-o, --output FORMAT``` : 输出格式(text, json)
- ```-h, --help``` : 显示帮助信息

### 交互式控制

在交互模式下运行（默认）时，可以使用以下键盘快捷键：
- ```r``` : 立即刷新
- ```t``` : 循环切换主题
- ```h``` : 显示帮助
- ```q``` : 退出（在连续模式下）

## 主题
Ping-Status 内置三种主题：
1. **Tokyo Night**: 受Tokyo-night启发的深色主题（默认）
2. **Adiwata**: 带有蓝色调的平衡深色主题
3. **Apple System**: 模仿 macOS 系统颜色的浅色主题

## 贡献

欢迎贡献代码！请随时提交 Pull Request。对于重大更改，请先开一个 issue 讨论您想要更改的内容。

## 许可证

本项目采用 NC-OSL 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 致谢
- 灵感来自 httpstat 和类似的 HTTP 调试工具
- UI 设计受 btop 和其他现代终端应用程序启发
- 颜色主题受 Tokyo Night 等流行配色方案启发