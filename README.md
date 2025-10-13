# Ping-Status v2.0

一款现代化的命令行HTTP请求分析工具，具有美观的可主题化界面、详细的时序信息显示和强大的数据包追踪功能。

![Ping-Status 界面预览](https://via.placeholder.com/800x400/1a1b26/bb9af7?text=Ping-Status+UI)

## ✨ 功能特点

### 🚀 核心功能
- **详细的 HTTP 时序分析**：测量并显示 DNS 查询、TCP 连接、TLS 握手、服务器处理和内容传输时间
- **美观的可主题化界面**：受现代系统监视器（如 btop）启发的多种颜色主题
- **实时可视化**：进度条和历史图表用于跟踪性能变化
- **数据包追踪**：捕获和分析网络数据包，提供详细的网络层信息
- **JSON 输出支持**：机器可读的输出格式，便于脚本和自动化

### 🔧 高级特性
- **模块化架构**：清晰的代码组织，便于维护和扩展
- **多种显示模式**：支持美化界面、树形图和JSON输出
- **连续监控**：可配置间隔时间持续监控端点
- **可定制配置**：广泛的外观和行为配置选项
- **多主题支持**：内置多种精美主题，可自定义配色
- **智能错误处理**：完善的错误检测和恢复机制
- **内存安全**：优化的内存管理和资源清理

## 📦 安装

### 系统要求
- Linux/Unix 系统
- C 编译器 (gcc/clang)
- ncurses 开发库
- make 构建工具

### 安装步骤

1. **克隆项目**
```bash
git clone https://github.com/your-repo/ping-status.git
cd ping-status
```

2. **构建项目**
```bash
make
```

3. **安装程序**（可选）
```bash
sudo make install
```

### 开发构建
```bash
# 开发模式（带调试信息）
make debug

# 发布模式（优化构建）
make release
```

## ⚙️ 配置

首次运行时，程序会自动在 `~/.config/ping-status/` 创建配置文件。

### 配置文件位置
- 主配置文件：`~/.config/ping-status/config.conf`
- 主题配置文件：`~/.config/ping-status/css.conf`

### 配置选项

```ini
[general]
# 默认检查间隔（秒，用于连续模式）
interval = 5

# 默认请求次数
count = 1

# 默认输出格式 (text, json)
output_format = text

# 默认主题 (tokyo-night, adiwata, apple-system)
theme = tokyo-night

# HTTP 请求超时时间（秒）
timeout = 10

[display]
# UI 刷新率（毫秒）
refresh_rate = 100

# 是否显示进度条
show_progress_bars = 1

# 是否显示历史图表
show_history = 1

# 保留的历史记录最大条目数
max_history_entries = 50

[behavior]
# 是否跟随重定向
follow_redirects = 1

# 最大重定向次数
max_redirects = 10
```

## 🚀 使用方法

### 基本用法

```bash
# 基本使用
ping-status https://example.com

# 指定请求次数
ping-status -c 5 https://example.com

# 连续监控
ping-status -C https://api.example.com
```

### 高级用法

```bash
# 自定义间隔和次数
ping-status -i 10 -c 3 https://httpbin.org/get

# JSON 输出（用于脚本）
ping-status -j -c 1 https://api.example.com

# 树形图显示模式
ping-status -T https://example.com

# 美化界面模式（默认）
ping-status -v https://example.com

# 组合使用
ping-status -v -i 2 -C https://example.com
```

### 命令行选项

| 选项 | 长选项 | 描述 | 默认值 |
|------|--------|------|--------|
| `-i` | `--interval` | 请求间隔（秒） | 5 |
| `-c` | `--count` | 请求次数 | 1 |
| `-C` | `--continuous` | 连续模式 | 否 |
| `-v` | `--verbose` | 详细输出 | 否 |
| `-t` | `--theme` | 颜色主题 | tokyo-night |
| `-j` | `--json` | JSON输出格式 | 否 |
| `-T` | `--tree` | 树形图显示模式 | 否 |
| `--timeout` | 请求超时时间（秒） | 10 |
| `-h` | `--help` | 显示帮助信息 | - |
| `-V` | `--version` | 显示版本信息 | - |

### 内置主题

Ping-Status 提供三种精美主题：

1. **Tokyo Night** 🌙 - 受Tokyo Night启发的深色主题（默认）
2. **Adiwata** 🌊 - 带有蓝色调的平衡深色主题
3. **Apple System** 🍎 - 模仿 macOS 系统颜色的浅色主题

## 📊 输出示例

### 🎨 美化界面模式
```
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║  🚀 Ping-Status 2.0.0 - HTTP网络分析工具 🚀
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

┌─ 目标信息 ─────────────────────────────────────────────────────┐
│ 🌐 URL: https://example.com
└─────────────────────────────────────────────────────────────────┘

┌─ HTTP时序分析 ─────────────────────────────────────────────────┐
🔍 DNS查询:    15.23 ms █░░░░░░░░░░░░░░░░░░░
🔗 TCP连接:    25.41 ms ██░░░░░░░░░░░░░░░░░░
🔒 TLS握手:    45.12 ms ███░░░░░░░░░░░░░░░░░
⚙️ 服务器处理:   85.67 ms █████░░░░░░░░░░░░░░░
📦 内容传输:    45.23 ms ███░░░░░░░░░░░░░░░░░
└─────────────────────────────────────────────────────────────────┘

┌─ 数据包追踪 (8个数据包) ───────────────────────────────────────┐
│ 📦 192.168.1.127:55904 → 10.0.0.181:206   [TCP] 608 bytes │
│ 📦 192.168.1.124:53445 → 10.0.0.73:386   [TCP] 958 bytes │
│ 📦 192.168.1.136:52952 → 10.0.0.67:769   [HTTP] 272 bytes │
│ 📦 192.168.1.148:52276 → 10.0.0.174:137   [ICMP] 364 bytes │
└─────────────────────────────────────────────────────────────────┘

┌─ 连接摘要 ─────────────────────────────────────────────────────┐
│ 📊 HTTP状态: 200 [✅ 成功]
│ 📥 下载大小: 43.9KB
│ ⚡ 平均速度: 202.4 KB/s
│ 📈 数据包数: 8
└─────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════
```

### 🌳 树形图模式
```
🌳 Ping-Status 树形网络分析视图
═══════════════════════════════════════════════════════

📡 https://example.com
└── 🔗 HTTP请求
    ├── ⏱️  时序分析
    │   ├── 🔍 DNS查询: 15ms
    │   ├── 🔗 TCP连接: 25ms
    │   ├── 🔒 TLS握手: 45ms
    │   ├── ⚙️  服务器处理: 86ms
    │   └── 📦 内容传输: 45ms
    ├── 📦 数据包追踪 (8个)
    │   ├── 📡 192.168.1.127:55904 → 10.0.0.181:206
    │   │   ├── 🔖 协议: TCP
    │   │   ├── 📏 大小: 608.0B
    │   │   └── 📄 负载: GET / HTTP/1.1
    │   └── ... 还有 7 个数据包
    └── 📊 连接信息
        ├── 🌐 状态码: 200
        ├── 📥 下载大小: 43.9KB
        ├── ⚡ 平均速度: 202.4 KB/s
        └── 📄 内容类型: text/html; charset=UTF-8
```

### 📋 JSON 输出模式
```json
{
  "namelookup_time": 0.015,
  "connect_time": 0.041,
  "appconnect_time": 0.086,
  "pretransfer_time": 0.096,
  "starttransfer_time": 0.182,
  "total_time": 0.217,
  "http_code": 200,
  "effective_url": "https://example.com",
  "content_type": "text/html; charset=UTF-8",
  "size_download": 43856,
  "speed_download": 202112,
  "packet_count": 8
}
```

## 🔧 开发

### 📁 项目结构
```
ping-status/
├── src/                    # 源代码目录
│   ├── main.c             # 主程序入口
│   ├── network.c          # 网络测量模块
│   ├── ui.c               # 用户界面模块
│   ├── config.c           # 配置管理模块
│   └── utils.c            # 工具函数模块
├── include/               # 头文件目录
│   ├── types.h            # 类型定义
│   ├── network.h          # 网络模块接口
│   ├── ui.h               # UI模块接口
│   ├── config.h           # 配置模块接口
│   └── utils.h            # 工具函数接口
├── docs/                  # 文档目录
│   ├── README.md          # 英文文档
│   └── ZH.README.md       # 中文文档
├── Makefile              # 模块化构建系统
├── LICENSE               # MIT许可证
└── .gitignore            # Git忽略文件
```

### 构建选项

| 目标 | 描述 |
|------|------|
| `make` | 标准构建 |
| `make debug` | 开发构建（带调试信息） |
| `make release` | 发布构建（优化） |
| `make test` | 运行基本测试 |
| `make install` | 安装程序 |
| `make clean` | 清理构建文件 |

### 代码质量工具

```bash
# 格式化代码（需要安装 indent）
make format

# 静态分析（需要安装 cppcheck）
make analyze
```

## 🤝 贡献

欢迎贡献代码！请遵循以下步骤：

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 贡献指南
- 遵循现有的代码风格
- 为新功能添加测试
- 更新相关文档
- 确保所有测试通过

## 📝 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- **灵感来源**：httpstat 和类似的 HTTP 调试工具
- **UI 设计**：受 btop 和其他现代终端应用程序启发
- **颜色主题**：受 Tokyo Night、Adiwata 等流行配色方案启发
- **社区贡献**：感谢所有为项目做出贡献的开发者

## 📞 支持

如果您遇到问题或有建议，请：

1. 查看现有的 Issues：[GitHub Issues](https://github.com/your-repo/ping-status/issues)
2. 开启新的 Issue 描述您的问题
3. 发送邮件至：support@ping-status.dev

## 🗺️ 路线图

- [ ] HTTP/2 和 HTTP/3 支持
- [ ] 自定义数据包过滤器
- [ ] 导出报告功能（PDF/HTML）
- [ ] Web 界面版本
- [ ] 国际化支持
- [ ] 插件系统
- [ ] 实时图表显示
- [ ] 性能基准测试工具
- [ ] 网络拓扑可视化

---

**Ping-Status** - 让网络调试更简单、更直观！ 🌟
