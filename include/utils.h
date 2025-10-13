/**
 * Ping-Status - 工具函数模块头文件
 */

#ifndef UTILS_H
#define UTILS_H

#include "types.h"

// 工具函数
char* my_strdup(const char* s);
void* safe_malloc(size_t size);
void handle_signal(int sig);

// 字符串处理
void trim_string(char* str);
int string_starts_with(const char* str, const char* prefix);
int string_ends_with(const char* str, const char* suffix);

// 时间和格式化
char* format_time(double seconds);
char* format_size(long long bytes);
char* format_speed(double bytes_per_second);

// 数学计算
double calculate_percentage(double value, double total);
int round_to_int(double value);

#endif