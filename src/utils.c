/**
 * Ping-Status - 工具函数模块
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include "../include/utils.h"
#include "../include/types.h"

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

// 安全的内存分配
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// 信号处理
void handle_signal(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        // 这里可以添加清理代码
        exit(EXIT_SUCCESS);
    }
}

// 字符串处理函数
void trim_string(char* str) {
    if (!str) return;

    char* start = str;
    char* end;

    // 去除开头空格
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }

    // 去除结尾空格
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }

    *(end + 1) = '\0';

    // 移动字符串到开头
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int string_starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return 0;
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);

    return str_len >= prefix_len && memcmp(str, prefix, prefix_len) == 0;
}

int string_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return 0;
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return str_len >= suffix_len &&
           memcmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

// 时间格式化
char* format_time(double seconds) {
    static char buffer[32];

    if (seconds < 1.0) {
        snprintf(buffer, sizeof(buffer), "%.0fms", seconds * 1000);
    } else if (seconds < 60.0) {
        snprintf(buffer, sizeof(buffer), "%.2fs", seconds);
    } else {
        int minutes = (int)seconds / 60;
        double remaining_seconds = seconds - minutes * 60;
        snprintf(buffer, sizeof(buffer), "%dm%02.0fs", minutes, remaining_seconds);
    }

    return buffer;
}

// 大小格式化
char* format_size(long long bytes) {
    static char buffer[32];
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size = (double)bytes;

    while (size >= 1024 && unit_index < 4) {
        size /= 1024;
        unit_index++;
    }

    snprintf(buffer, sizeof(buffer), "%.1f%s", size, units[unit_index]);
    return buffer;
}

// 速度格式化
char* format_speed(double bytes_per_second) {
    static char buffer[32];

    if (bytes_per_second < 1024) {
        snprintf(buffer, sizeof(buffer), "%.0f B/s", bytes_per_second);
    } else if (bytes_per_second < 1024 * 1024) {
        snprintf(buffer, sizeof(buffer), "%.1f KB/s", bytes_per_second / 1024);
    } else {
        snprintf(buffer, sizeof(buffer), "%.1f MB/s", bytes_per_second / (1024 * 1024));
    }

    return buffer;
}

// 百分比计算
double calculate_percentage(double value, double total) {
    if (total == 0) return 0;
    return (value / total) * 100.0;
}

// 四舍五入
int round_to_int(double value) {
    return (int)(value + 0.5);
}