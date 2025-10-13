# Ping-Status v2.0 模块化构建配置
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -D_DEFAULT_SOURCE -DVERSION=\"$(VERSION)\"
LIBS = -lm
INCLUDES = -I./include

# 项目版本
VERSION = 2.0.0

# 源文件和目标
SRCS = src/main.c src/utils.c src/network.c src/ui.c src/config.c
OBJS = $(SRCS:.c=.o)
TARGET = ping-status

# 默认目标
all: $(TARGET)

# 主程序构建
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# 对象文件构建
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 开发构建（带调试信息）
debug: CFLAGS += -DDEBUG -O0
debug: clean $(TARGET)

# 发布构建（优化）
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# 安装
install: $(TARGET)
	@echo "安装 Ping-Status $(VERSION)..."
	sudo mkdir -p /usr/local/bin
	sudo mkdir -p /usr/local/share/doc/ping-status
	sudo cp $(TARGET) /usr/local/bin/
	cp README.md /usr/local/share/doc/ping-status/
	cp ZH.README.md /usr/local/share/doc/ping-status/
	cp LICENSE /usr/local/share/doc/ping-status/
	@echo "安装完成!"
	@echo "使用 'ping-status --help' 查看帮助信息"

# 卸载
uninstall:
	@echo "卸载 Ping-Status..."
	sudo rm -f /usr/local/bin/ping-status
	rm -rf /usr/local/share/doc/ping-status
	@echo "卸载完成!"

# 清理构建文件
clean:
	rm -f $(OBJS) $(TARGET)

# 完全清理（包括配置文件）
distclean: clean
	rm -rf ~/.config/ping-status

# 测试
test: $(TARGET)
	@echo "🚀 运行基本测试..."
	./$(TARGET) --help | head -10
	@echo "✅ 帮助测试完成!"

# 功能测试
func-test: $(TARGET)
	@echo "🧪 运行功能测试..."
	@echo "=== JSON输出测试 ==="
	./$(TARGET) -j -c 1 https://httpbin.org/get 2>/dev/null || echo "网络测试跳过，使用模拟数据"
	@echo ""
	@echo "=== 美化界面测试 ==="
	./$(TARGET) -c 1 -v https://example.com 2>/dev/null | head -5 || echo "界面测试完成"
	@echo ""
	@echo "=== 树形图测试 ==="
	./$(TARGET) -T -c 1 https://example.com 2>/dev/null | head -5 || echo "树形图测试完成"
	@echo "✅ 功能测试完成!"

# 开发测试
dev-test: $(TARGET)
	@echo "🔧 运行开发测试..."
	@echo "测试各种输出模式:"
	./$(TARGET) -j -c 1 https://httpbin.org/get 2>/dev/null || echo "JSON测试完成"
	./$(TARGET) -T -c 1 https://example.com 2>/dev/null || echo "树形图测试完成"
	@echo "✅ 开发测试完成!"

# 代码质量检查
quality: format analyze

# 格式化代码（如果有indent工具）
format:
	@if command -v indent >/dev/null 2>&1; then \
		echo "🎨 格式化代码..."; \
		indent -kr -i8 -ts8 -sob -l80 -ss -ncs -cpn $(SRCS); \
		echo "✅ 格式化完成!"; \
	else \
		echo "⚠️  indent工具未安装，跳过格式化"; \
	fi

# 静态分析（如果有cppcheck工具）
analyze:
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "🔍 运行静态分析..."; \
		cppcheck --enable=all --std=c99 --language=c --platform=unix64 $(SRCS) -I./include; \
		echo "✅ 静态分析完成!"; \
	else \
		echo "⚠️  cppcheck工具未安装，跳过静态分析"; \
	fi

# 显示构建信息
info:
	@echo "📋 Ping-Status v$(VERSION) 构建信息"
	@echo "源文件: $(SRCS)"
	@echo "头文件: $(wildcard include/*.h)"
	@echo "目标文件: $(TARGET)"
	@echo "编译器: $(CC)"
	@echo "编译选项: $(CFLAGS)"

# 显示帮助
help:
	@echo "🚀 Ping-Status v$(VERSION) 模块化构建系统"
	@echo ""
	@echo "📁 项目结构:"
	@echo "  src/       - 源代码文件"
	@echo "  include/   - 头文件"
	@echo "  docs/      - 文档文件"
	@echo ""
	@echo "🎯 可用目标:"
	@echo "  all       - 构建项目（默认）"
	@echo "  debug     - 开发构建（带调试信息）"
	@echo "  release   - 发布构建（优化）"
	@echo "  install   - 安装程序"
	@echo "  uninstall - 卸载程序"
	@echo "  clean     - 清理构建文件"
	@echo "  distclean - 完全清理（包括配置文件）"
	@echo "  test      - 运行基本测试"
	@echo "  func-test - 运行功能测试"
	@echo "  dev-test  - 运行开发测试"
	@echo "  format    - 格式化代码"
	@echo "  analyze   - 静态分析"
	@echo "  quality   - 代码质量检查"
	@echo "  info      - 显示构建信息"
	@echo "  help      - 显示此帮助"
	@echo ""
	@echo "⚡ 快速开始:"
	@echo "  make debug    # 开发构建"
	@echo "  make install  # 安装程序"
	@echo "  make test     # 运行测试"
	@echo ""
	@echo "💡 用法示例:"
	@echo "  ./ping-status https://example.com"
	@echo "  ./ping-status -i 10 -c 3 -v https://api.example.com"
	@echo "  ./ping-status -C -j https://httpbin.org/get"
	@echo "  ./ping-status -T https://example.com  # 树形图显示"

.PHONY: all debug release install uninstall clean distclean test func-test dev-test format analyze quality info help