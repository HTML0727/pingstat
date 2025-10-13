/**
 * Ping-Status - 网络测量模块头文件
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "types.h"

// 网络测量函数
HTTPTiming* measure_http_timing(const char* url);
PacketInfo* generate_mock_packets(int* packet_count);
void print_timing_json(HTTPTiming* timing);

// 内存管理函数
void free_http_timing(HTTPTiming* timing);
TimingHistory* create_timing_history(int initial_capacity);
void add_timing_to_history(TimingHistory* history, HTTPTiming* timing);
void free_timing_history(TimingHistory* history);

#endif