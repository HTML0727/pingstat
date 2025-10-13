/**
 * Ping-Status - 用户界面模块
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/ui.h"
#include "../include/types.h"
#include "../include/utils.h"

// 全局UI状态（由主程序设置）
extern UIState ui_state;

// 颜色设置函数
void set_color(const char* color) {
    // 这里可以根据终端支持的颜色设置不同的颜色码
    if (strcmp(color, "red") == 0) {
        printf("\033[1;31m");
    } else if (strcmp(color, "green") == 0) {
        printf("\033[1;32m");
    } else if (strcmp(color, "yellow") == 0) {
        printf("\033[1;33m");
    } else if (strcmp(color, "blue") == 0) {
        printf("\033[1;34m");
    } else if (strcmp(color, "magenta") == 0) {
        printf("\033[1;35m");
    } else if (strcmp(color, "cyan") == 0) {
        printf("\033[1;36m");
    } else if (strcmp(color, "white") == 0) {
        printf("\033[1;37m");
    }
}

void reset_color() {
    printf("\033[0m");
}

// 打印标题
void print_header() {
    set_color("cyan");
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                  ║\n");
    printf("║  🚀 Ping-Status %s - HTTP网络分析工具 🚀\n", VERSION);
    printf("║                                                                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    reset_color();
}

// 打印底部分隔线
void print_footer() {
    set_color("cyan");
    printf("════════════════════════════════════════════════════════════════════\n");
    reset_color();
}

// 美化的ASCII艺术UI显示
void print_beautiful_ui(HTTPTiming* timing) {
    // 清屏并设置颜色（如果支持）
    printf("\033[2J\033[H"); // 清屏并移动光标到顶部

    print_header();

    // 目标信息
    set_color("yellow");
    printf("┌─ 目标信息 ─────────────────────────────────────────────────────┐\n");
    printf("│ 🌐 URL: %s\n", ui_state.url);
    printf("└─────────────────────────────────────────────────────────────────┘\n");
    reset_color();

    if (timing) {
        // HTTP时序分析 - 使用进度条样式
        set_color("magenta");
        printf("┌─ HTTP时序分析 ─────────────────────────────────────────────────┐\n");
        reset_color();

        double times[5] = {
            timing->namelookup_time * 1000,
            (timing->connect_time - timing->namelookup_time) * 1000,
            (timing->appconnect_time - timing->connect_time) * 1000,
            (timing->starttransfer_time - timing->pretransfer_time) * 1000,
            (timing->total_time - timing->starttransfer_time) * 1000
        };

        char* labels[5] = {"DNS查询", "TCP连接", "TLS握手", "服务器处理", "内容传输"};
        char* icons[5] = {"🔍", "🔗", "🔒", "⚙️", "📦"};

        for (int i = 0; i < 5; i++) {
            printf("%s %s: %8.2f ms ", icons[i], labels[i], times[i]);

            // 绘制迷你进度条
            int bar_length = 20;
            int filled = (times[i] / timing->total_time / 1000) * bar_length;
            if (filled > bar_length) filled = bar_length;

            set_color("green");
            for (int j = 0; j < filled; j++) printf("█");
            reset_color();

            for (int j = filled; j < bar_length; j++) printf("░");
            printf("\n");
        }

        set_color("cyan");
        printf("└─────────────────────────────────────────────────────────────────┘\n");
        reset_color();

        // 数据包追踪 - 使用表格样式
        set_color("blue");
        printf("┌─ 数据包追踪 (%d个数据包) ───────────────────────────────────────┐\n", timing->packet_count);
        reset_color();

        int display_count = timing->packet_count < 4 ? timing->packet_count : 4;
        for (int i = 0; i < display_count; i++) {
            PacketInfo* packet = &timing->packets[i];

            printf("│ 📦 %s:%-5d → %s:%-5d [%s] %4d bytes │\n",
                   packet->source_ip, packet->source_port,
                   packet->dest_ip, packet->dest_port,
                   packet->protocol, packet->packet_size);
        }

        // 填充剩余行
        for (int i = display_count; i < 4; i++) {
            printf("│                                                              │\n");
        }

        set_color("blue");
        printf("└─────────────────────────────────────────────────────────────────┘\n");
        reset_color();

        // 连接摘要 - 使用卡片样式
        set_color("green");
        printf("┌─ 连接摘要 ─────────────────────────────────────────────────────┐\n");
        reset_color();

        // HTTP状态
        printf("│ 📊 HTTP状态: %ld ", timing->http_code);
        if (timing->http_code >= 200 && timing->http_code < 300) {
            set_color("green");
            printf("[✅ 成功]");
            reset_color();
        } else if (timing->http_code >= 300 && timing->http_code < 400) {
            set_color("yellow");
            printf("[↩️  重定向]");
            reset_color();
        } else {
            set_color("red");
            printf("[❌ 错误]");
            reset_color();
        }
        printf("\n");

        printf("│ 📥 下载大小: %s\n", format_size(timing->size_download));
        printf("│ ⚡ 平均速度: %s\n", format_speed(timing->speed_download));
        printf("│ 📈 数据包数: %d\n", timing->packet_count);

        set_color("green");
        printf("└─────────────────────────────────────────────────────────────────┘\n");
        reset_color();
    }

    // 状态栏
    set_color("white");
    printf("┌─ 状态信息 ─────────────────────────────────────────────────────┐\n");
    printf("│ 📋 状态: %s", ui_state.status);
    if (ui_state.is_measuring) {
        set_color("yellow");
        printf(" [🔄 测量中...]");
        reset_color();
    }
    printf("\n│                                                              │\n");
    printf("└─────────────────────────────────────────────────────────────────┘\n");
    reset_color();

    print_footer();
}

// 树形图显示
void print_tree_view(HTTPTiming* timing, TimingHistory* history) {
    printf("\033[2J\033[H"); // 清屏

    set_color("cyan");
    printf("🌳 Ping-Status 树形网络分析视图\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    reset_color();

    if (!timing) {
        printf("暂无数据\n");
        return;
    }

    // 根节点
    printf("📡 %s\n", timing->effective_url);
    printf("└── 🔗 HTTP请求\n");

    // 时序分析树形结构
    printf("    ├── ⏱️  时序分析\n");
    printf("    │   ├── 🔍 DNS查询: %s\n", format_time(timing->namelookup_time));
    printf("    │   ├── 🔗 TCP连接: %s\n", format_time(timing->connect_time - timing->namelookup_time));
    printf("    │   ├── 🔒 TLS握手: %s\n", format_time(timing->appconnect_time - timing->connect_time));
    printf("    │   ├── ⚙️  服务器处理: %s\n", format_time(timing->starttransfer_time - timing->pretransfer_time));
    printf("    │   └── 📦 内容传输: %s\n", format_time(timing->total_time - timing->starttransfer_time));

    // 数据包追踪树形结构
    printf("    ├── 📦 数据包追踪 (%d个)\n", timing->packet_count);

    int display_count = timing->packet_count < 3 ? timing->packet_count : 3;
    for (int i = 0; i < display_count; i++) {
        PacketInfo* packet = &timing->packets[i];
        printf("    │   ├── 📡 %s:%d → %s:%d\n",
               packet->source_ip, packet->source_port,
               packet->dest_ip, packet->dest_port);
        printf("    │   │   ├── 🔖 协议: %s\n", packet->protocol);
        printf("    │   │   ├── 📏 大小: %s\n", format_size(packet->packet_size));
        printf("    │   │   └── 📄 负载: %s\n", packet->payload);
    }

    if (timing->packet_count > 3) {
        printf("    │   └── ... 还有 %d 个数据包\n", timing->packet_count - 3);
    }

    // 连接信息树形结构
    printf("    └── 📊 连接信息\n");
    printf("        ├── 🌐 状态码: %ld\n", timing->http_code);
    printf("        ├── 📥 下载大小: %s\n", format_size(timing->size_download));
    printf("        ├── ⚡ 平均速度: %s\n", format_speed(timing->speed_download));
    printf("        └── 📄 内容类型: %s\n", timing->content_type ? timing->content_type : "未知");

    printf("\n状态: %s", ui_state.status);
    if (ui_state.is_measuring) {
        set_color("yellow");
        printf(" [测量中...]");
        reset_color();
    }
    printf("\n");
}

// 紧凑视图显示
void print_compact_view(HTTPTiming* timing) {
    if (!timing) return;

    printf("📊 %s | ", format_time(timing->total_time));
    printf("📥 %s | ", format_size(timing->size_download));
    printf("⚡ %s | ", format_speed(timing->speed_download));
    printf("📦 %d包 | ", timing->packet_count);
    printf("🔢 %ld | ", timing->http_code);

    if (timing->http_code >= 200 && timing->http_code < 300) {
        set_color("green");
        printf("✅ 成功");
        reset_color();
    } else if (timing->http_code >= 300 && timing->http_code < 400) {
        set_color("yellow");
        printf("↩️ 重定向");
        reset_color();
    } else {
        set_color("red");
        printf("❌ 错误");
        reset_color();
    }
    printf("\n");
}

// 进度条显示
void print_progress_bar(double percentage, int width) {
    int filled = (int)(percentage * width / 100.0);

    set_color("green");
    for (int i = 0; i < filled; i++) {
        printf("█");
    }
    reset_color();

    for (int i = filled; i < width; i++) {
        printf("░");
    }

    printf(" %.1f%%", percentage);
}