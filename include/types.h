/**
 * Ping-Status - 类型定义头文件
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define VERSION "2.0.0"
#define MAX_URL_LENGTH 256
#define MAX_STATUS_LENGTH 128
#define MAX_THEME_NAME 50

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

// 时序历史结构体
typedef struct {
    HTTPTiming** timings;
    int count;
    int capacity;
} TimingHistory;

// 应用配置结构体
typedef struct {
    char* url;
    int interval;
    int count;
    bool continuous;
    bool verbose;
    bool json_output;
    bool tree_output;
    char* theme;
    int timeout;
    bool verify_ssl;
    bool follow_redirects;
    int max_redirects;
} AppConfig;

// UI状态结构体
typedef struct {
    char url[MAX_URL_LENGTH];
    char status[MAX_STATUS_LENGTH];
    bool is_measuring;
    int progress;
    int current_theme_index;
} UIState;

#endif