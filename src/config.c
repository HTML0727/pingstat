/**
 * Ping-Status - 配置管理模块
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include "../include/config.h"
#include "../include/types.h"
#include "../include/utils.h"

// 创建默认配置
AppConfig* create_default_config() {
    AppConfig* config = safe_malloc(sizeof(AppConfig));

    config->url = NULL;
    config->interval = 5;
    config->count = 1;
    config->continuous = false;
    config->verbose = false;
    config->json_output = false;
    config->tree_output = false;
    config->theme = my_strdup("tokyo-night");
    config->timeout = 10;
    config->verify_ssl = true;
    config->follow_redirects = true;
    config->max_redirects = 10;

    return config;
}

// 释放配置内存
void free_config(AppConfig* config) {
    if (config) {
        free(config->url);
        free(config->theme);
        free(config);
    }
}

// 创建配置目录
int create_config_directory() {
    struct passwd *pw = getpwuid(getuid());
    if (!pw) {
        fprintf(stderr, "无法获取用户信息\n");
        return 0;
    }

    const char *homedir = pw->pw_dir;

    char config_dir[256];
    snprintf(config_dir, sizeof(config_dir), "%s/.config/ping-status", homedir);

    if (mkdir(config_dir, 0755) == -1) {
        if (errno != EEXIST) {
            perror("创建配置目录失败");
            return 0;
        }
    }

    // 创建默认配置文件
    char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/config.conf", config_dir);

    if (access(config_path, F_OK) != 0) {
        FILE *config_file = fopen(config_path, "w");
        if (config_file) {
            fprintf(config_file, "# Ping-Status 配置文件\n");
            fprintf(config_file, "# 此文件控制 ping-status 应用程序的行为\n\n");

            fprintf(config_file, "[general]\n");
            fprintf(config_file, "# 默认检查间隔（秒，用于连续模式）\n");
            fprintf(config_file, "interval = 5\n\n");
            fprintf(config_file, "# 默认请求次数\n");
            fprintf(config_file, "count = 1\n\n");
            fprintf(config_file, "# 默认输出格式 (text, json)\n");
            fprintf(config_file, "output_format = text\n\n");
            fprintf(config_file, "# 默认主题 (tokyo-night, adiwata, apple-system)\n");
            fprintf(config_file, "theme = tokyo-night\n\n");
            fprintf(config_file, "# HTTP 请求超时时间（秒）\n");
            fprintf(config_file, "timeout = 10\n\n");

            fprintf(config_file, "[display]\n");
            fprintf(config_file, "# UI 刷新率（毫秒）\n");
            fprintf(config_file, "refresh_rate = 100\n\n");
            fprintf(config_file, "# 是否显示进度条\n");
            fprintf(config_file, "show_progress_bars = 1\n\n");
            fprintf(config_file, "# 是否显示历史图表\n");
            fprintf(config_file, "show_history = 1\n\n");
            fprintf(config_file, "# 保留的历史记录最大条目数\n");
            fprintf(config_file, "max_history_entries = 50\n\n");

            fprintf(config_file, "[behavior]\n");
            fprintf(config_file, "# 是否跟随重定向\n");
            fprintf(config_file, "follow_redirects = 1\n\n");
            fprintf(config_file, "# 最大重定向次数\n");
            fprintf(config_file, "max_redirects = 10\n\n");

            fclose(config_file);
        }
    }

    return 1;
}

// 读取配置文件
void read_config_file(AppConfig* config) {
    struct passwd *pw = getpwuid(getuid());
    if (!pw) return;

    const char *homedir = pw->pw_dir;

    char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/.config/ping-status/config.conf", homedir);

    FILE* file = fopen(config_path, "r");
    if (!file) return;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 跳过注释和空行
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        // 简单解析配置文件（实际应该使用更完善的解析器）
        trim_string(line);

        if (strncmp(line, "interval =", 10) == 0) {
            config->interval = atoi(line + 10);
        } else if (strncmp(line, "count =", 7) == 0) {
            config->count = atoi(line + 7);
        } else if (strncmp(line, "theme =", 7) == 0) {
            char* theme_start = line + 7;
            trim_string(theme_start);
            if (strlen(theme_start) > 0) {
                free(config->theme);
                config->theme = my_strdup(theme_start);
            }
        } else if (strncmp(line, "timeout =", 9) == 0) {
            config->timeout = atoi(line + 9);
        }
    }

    fclose(file);
}

// 命令行参数解析
AppConfig* parse_arguments(int argc, char *argv[]) {
    AppConfig* config = create_default_config();

    // 读取配置文件
    create_config_directory();
    read_config_file(config);

    int opt;
    static struct option long_options[] = {
        {"interval", required_argument, 0, 'i'},
        {"count", required_argument, 0, 'c'},
        {"continuous", no_argument, 0, 'C'},
        {"verbose", no_argument, 0, 'v'},
        {"theme", required_argument, 0, 't'},
        {"json", no_argument, 0, 'j'},
        {"tree", no_argument, 0, 'T'},
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
                config->tree_output = true;
                break;
            case 'h':
                print_usage();
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
        print_usage();
        free_config(config);
        return NULL;
    }

    return config;
}

// 显示使用帮助
void print_usage() {
    printf("🚀 Ping-Status %s - 现代化的HTTP请求分析工具\n\n", VERSION);
    printf("用法：ping-status [选项] URL\n\n");
    printf("选项：\n");
    printf("  -i, --interval 秒数     请求间隔（默认：5）\n");
    printf("  -c, --count 次数        请求次数（默认：1）\n");
    printf("  -C, --continuous        连续模式\n");
    printf("  -v, --verbose           详细输出\n");
    printf("  -t, --theme 主题        颜色主题\n");
    printf("  -j, --json              JSON输出\n");
    printf("  -T, --tree              树形图显示\n");
    printf("  --timeout 秒数          请求超时\n");
    printf("  -h, --help              显示帮助\n");
    printf("  -V, --version           显示版本\n\n");
    printf("示例：\n");
    printf("  ping-status https://example.com\n");
    printf("  ping-status -i 10 -c 5 -v https://api.example.com\n");
    printf("  ping-status -C -j https://httpbin.org/get\n");
    printf("  ping-status -T https://example.com  # 树形图显示\n");
}