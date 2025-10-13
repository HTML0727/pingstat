/**
 * Ping-Status 简化版 - 专注核心功能演示
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <getopt.h>

// strdup替代函数
char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* new_s = malloc(len);
    if (new_s) {
        memcpy(new_s, s, len);
    }
    return new_s;
}

#define VERSION "2.0.0"

// 数据包追踪结构体
typedef struct {
    char source_ip[16];
    char dest_ip[16];
    int source_port;
    int dest_port;
    char protocol[8];
    int packet_size;
    double timestamp;
    char payload[256];
} PacketInfo;

// HTTP时序结构体
typedef struct {
    double namelookup_time;
    double connect_time;
    double appconnect_time;
    double pretransfer_time;
    double starttransfer_time;
    double total_time;
    long http_code;
    char* effective_url;
    char* content_type;
    long long size_download;
    long long speed_download;
    PacketInfo* packets;
    int packet_count;
} HTTPTiming;

// 应用配置结构体
typedef struct {
    char* url;
    int interval;
    int count;
    bool continuous;
    bool verbose;
    bool json_output;
    char* theme;
    int timeout;
} AppConfig;

// 全局变量
volatile sig_atomic_t stop_requested = 0;

void handle_signal(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        stop_requested = 1;
    }
}

// 安全的字符串复制

// 生成模拟数据包
PacketInfo* generate_mock_packets(int* packet_count) {
    *packet_count = 5 + rand() % 10;
    PacketInfo* packets = malloc(sizeof(PacketInfo) * (*packet_count));
    if (!packets) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }

    const char* protocols[] = {"TCP", "UDP", "ICMP", "HTTP", "HTTPS"};
    const char* payloads[] = {
        "GET / HTTP/1.1",
        "Host: example.com",
        "User-Agent: ping-status/" VERSION,
        "Accept: text/html",
        "Connection: close"
    };

    for (int i = 0; i < *packet_count; i++) {
        snprintf(packets[i].source_ip, sizeof(packets[i].source_ip), "192.168.1.%d", 100 + rand() % 50);
        snprintf(packets[i].dest_ip, sizeof(packets[i].dest_ip), "10.0.0.%d", rand() % 255);
        packets[i].source_port = 50000 + rand() % 15000;
        packets[i].dest_port = rand() % 1000 + 1;
        snprintf(packets[i].protocol, sizeof(packets[i].protocol), "%s", protocols[rand() % 5]);
        packets[i].packet_size = 64 + rand() % 1400;
        packets[i].timestamp = time(NULL) + (rand() % 100) / 1000.0;
        snprintf(packets[i].payload, sizeof(packets[i].payload), "%s", payloads[rand() % 5]);
    }

    return packets;
}

// HTTP时序测量（带数据包追踪）
HTTPTiming* measure_http_timing(const char* url) {
    HTTPTiming* timing = malloc(sizeof(HTTPTiming));
    if (!timing) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }

    // 模拟时序数据
    timing->namelookup_time = 0.01 + (rand() % 100) / 1000.0;
    timing->connect_time = timing->namelookup_time + 0.02 + (rand() % 50) / 1000.0;
    timing->appconnect_time = timing->connect_time + 0.05 + (rand() % 100) / 1000.0;
    timing->pretransfer_time = timing->appconnect_time + 0.01;
    timing->starttransfer_time = timing->pretransfer_time + 0.1 + (rand() % 200) / 1000.0;
    timing->total_time = timing->starttransfer_time + 0.05 + (rand() % 100) / 1000.0;

    timing->http_code = 200 + rand() % 100;
    timing->effective_url = my_strdup(url);
    timing->content_type = my_strdup("text/html; charset=UTF-8");
    timing->size_download = 1024 + rand() % 50000;
    timing->speed_download = timing->size_download / timing->total_time;

    // 生成数据包追踪信息
    timing->packets = generate_mock_packets(&timing->packet_count);

    return timing;
}

// 内存管理
void free_http_timing(HTTPTiming* timing) {
    if (timing) {
        free(timing->effective_url);
        free(timing->content_type);
        free(timing->packets);
        free(timing);
    }
}

// 配置管理
AppConfig* create_default_config() {
    AppConfig* config = malloc(sizeof(AppConfig));
    if (!config) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }

    config->url = NULL;
    config->interval = 5;
    config->count = 1;
    config->continuous = false;
    config->verbose = false;
    config->json_output = false;
    config->theme = my_strdup("tokyo-night");
    config->timeout = 10;

    return config;
}

void free_config(AppConfig* config) {
    if (config) {
        free(config->url);
        free(config->theme);
        free(config);
    }
}

// 命令行参数解析
AppConfig* parse_arguments(int argc, char *argv[]) {
    AppConfig* config = create_default_config();

    int opt;
    static struct option long_options[] = {
        {"interval", required_argument, 0, 'i'},
        {"count", required_argument, 0, 'c'},
        {"continuous", no_argument, 0, 'C'},
        {"verbose", no_argument, 0, 'v'},
        {"theme", required_argument, 0, 't'},
        {"json", no_argument, 0, 'j'},
        {"timeout", required_argument, 0, 'T'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "i:c:Cvt:jT:hV", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                config->interval = atoi(optarg);
                break;
            case 'c':
                config->count = atoi(optarg);
                break;
            case 'C':
                config->continuous = true;
                break;
            case 'v':
                config->verbose = true;
                break;
            case 't':
                free(config->theme);
                config->theme = my_strdup(optarg);
                break;
            case 'j':
                config->json_output = true;
                break;
            case 'T':
                config->timeout = atoi(optarg);
                break;
            case 'h':
                printf("Ping-Status %s - 现代化的HTTP请求分析工具\n\n", VERSION);
                printf("用法：ping-status [选项] URL\n\n");
                printf("选项：\n");
                printf("  -i, --interval 秒数     请求间隔（默认：5）\n");
                printf("  -c, --count 次数        请求次数（默认：1）\n");
                printf("  -C, --continuous        连续模式\n");
                printf("  -v, --verbose           详细输出\n");
                printf("  -t, --theme 主题        颜色主题\n");
                printf("  -j, --json              JSON输出\n");
                printf("  -T, --timeout 秒数      请求超时\n");
                printf("  -h, --help              显示帮助\n");
                printf("  -V, --version           显示版本\n\n");
                printf("示例：\n");
                printf("  ping-status https://example.com\n");
                printf("  ping-status -i 10 -c 5 https://api.example.com\n");
                free_config(config);
                return NULL;
            case 'V':
                printf("Ping-Status 版本 %s\n", VERSION);
                printf("一个现代化的HTTP网络分析工具，带数据包追踪功能\n");
                free_config(config);
                return NULL;
            default:
                printf("未知选项，请使用 -h 查看帮助\n");
                free_config(config);
                return NULL;
        }
    }

    if (optind < argc) {
        config->url = my_strdup(argv[optind]);
    } else {
        fprintf(stderr, "错误：需要提供URL\n\n");
        printf("用法：ping-status [选项] URL\n");
        printf("示例：ping-status https://example.com\n");
        free_config(config);
        return NULL;
    }

    return config;
}

// JSON输出
void print_timing_json(HTTPTiming* timing) {
    printf("{\n");
    printf("  \"namelookup_time\": %.3f,\n", timing->namelookup_time);
    printf("  \"connect_time\": %.3f,\n", timing->connect_time);
    printf("  \"appconnect_time\": %.3f,\n", timing->appconnect_time);
    printf("  \"pretransfer_time\": %.3f,\n", timing->pretransfer_time);
    printf("  \"starttransfer_time\": %.3f,\n", timing->starttransfer_time);
    printf("  \"total_time\": %.3f,\n", timing->total_time);
    printf("  \"http_code\": %ld,\n", timing->http_code);
    printf("  \"effective_url\": \"%s\",\n", timing->effective_url ? timing->effective_url : "");
    printf("  \"content_type\": \"%s\",\n", timing->content_type ? timing->content_type : "");
    printf("  \"size_download\": %lld,\n", (long long)timing->size_download);
    printf("  \"speed_download\": %lld,\n", (long long)timing->speed_download);
    printf("  \"packet_count\": %d\n", timing->packet_count);
    printf("}\n");
}

// 主程序
int main(int argc, char *argv[]) {
    srand(time(NULL));

    // 设置信号处理
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // 解析命令行参数
    AppConfig* config = parse_arguments(argc, argv);
    if (!config) {
        return EXIT_FAILURE;
    }

    printf("Ping-Status %s - 开始监控 %s\n", VERSION, config->url);
    if (config->verbose) {
        printf("配置: 间隔=%d秒, 计数=%d, 连续=%s\n",
               config->interval, config->count, config->continuous ? "是" : "否");
    }

    int count = 0;
    while (!stop_requested && (config->continuous || count < config->count)) {
        if (config->verbose) {
            printf("\n--- 第 %d 次测量 ---\n", count + 1);
        }

        // 执行测量
        HTTPTiming* timing = measure_http_timing(config->url);

        if (timing) {
            // 输出结果
            if (config->json_output) {
                print_timing_json(timing);
            } else {
                // 清屏并设置颜色（如果支持）
                printf("\033[2J\033[H"); // 清屏并移动光标到顶部

                // 顶部横幅
                printf("\033[1;36m"); // 亮青色
                printf("╔══════════════════════════════════════════════════════════════════╗\n");
                printf("║                                                                  ║\n");
                printf("║  🚀 Ping-Status %s - HTTP网络分析工具 🚀\n", VERSION);
                printf("║                                                                  ║\n");
                printf("╚══════════════════════════════════════════════════════════════════╝\n");
                printf("\033[0m"); // 重置颜色

                // 目标信息
                printf("\033[1;33m"); // 黄色
                printf("┌─ 目标信息 ─────────────────────────────────────────────────────┐\n");
                printf("│ 🌐 URL: %s\n", config->url);
                printf("└─────────────────────────────────────────────────────────────────┘\n");
                printf("\033[0m"); // 重置颜色

                // HTTP时序分析 - 使用进度条样式
                printf("\033[1;35m"); // 紫红色
                printf("┌─ HTTP时序分析 ─────────────────────────────────────────────────┐\n");
                printf("\033[0m"); // 重置颜色

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

                    printf("\033[1;32m"); // 绿色
                    for (int j = 0; j < filled; j++) printf("█");
                    printf("\033[0m"); // 重置颜色

                    for (int j = filled; j < bar_length; j++) printf("░");
                    printf("\n");
                }

                printf("\033[1;36m"); // 青色
                printf("└─────────────────────────────────────────────────────────────────┘\n");
                printf("\033[0m"); // 重置颜色

                // 数据包追踪 - 使用表格样式
                printf("\033[1;34m"); // 蓝色
                printf("┌─ 数据包追踪 (%d个数据包) ───────────────────────────────────────┐\n", timing->packet_count);
                printf("\033[0m"); // 重置颜色

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

                printf("\033[1;34m"); // 蓝色
                printf("└─────────────────────────────────────────────────────────────────┘\n");
                printf("\033[0m"); // 重置颜色

                // 连接摘要 - 使用卡片样式
                printf("\033[1;32m"); // 绿色
                printf("┌─ 连接摘要 ─────────────────────────────────────────────────────┐\n");
                printf("\033[0m"); // 重置颜色

                // HTTP状态
                printf("│ 📊 HTTP状态: %ld ", timing->http_code);
                if (timing->http_code >= 200 && timing->http_code < 300) {
                    printf("\033[1;32m[✅ 成功]\033[0m");
                } else if (timing->http_code >= 300 && timing->http_code < 400) {
                    printf("\033[1;33m[↩️  重定向]\033[0m");
                } else {
                    printf("\033[1;31m[❌ 错误]\033[0m");
                }
                printf("\n");

                printf("│ 📥 下载大小: %lld bytes\n", (long long)timing->size_download);
                printf("│ ⚡ 平均速度: %.1f KB/s\n", (double)timing->speed_download / 1024);
                printf("│ 📈 数据包数: %d\n", timing->packet_count);

                printf("\033[1;32m"); // 绿色
                printf("└─────────────────────────────────────────────────────────────────┘\n");
                printf("\033[0m"); // 重置颜色

                // 底部装饰
                printf("\033[1;36m"); // 青色
                printf("════════════════════════════════════════════════════════════════════\n");
                printf("\033[0m"); // 重置颜色
            }

            free_http_timing(timing);
        } else {
            printf("测量失败\n");
        }

        count++;

        // 等待间隔（除非是最后一次请求）
        if (!stop_requested && (config->continuous || count < config->count)) {
            for (int i = 0; i < config->interval && !stop_requested; i++) {
                sleep(1);
            }
        }
    }

    printf("\n测量完成! 总请求数: %d\n", count);

    free_config(config);
    return EXIT_SUCCESS;
}