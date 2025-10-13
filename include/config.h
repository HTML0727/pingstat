/**
 * Ping-Status - 配置管理模块头文件
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

// 配置管理函数
AppConfig* create_default_config();
void free_config(AppConfig* config);
AppConfig* parse_arguments(int argc, char *argv[]);
void print_usage();

// 配置文件操作
int create_config_directory();
void read_config_file(AppConfig* config);
void write_config_file(AppConfig* config);

#endif