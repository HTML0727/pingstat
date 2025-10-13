/**
 * Ping-Status - 网络测量模块
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/network.h"
#include "../include/types.h"
#include "../include/utils.h"

// 生成模拟数据包
PacketInfo* generate_mock_packets(int* packet_count) {
    *packet_count = 5 + rand() % 10;
    PacketInfo* packets = safe_malloc(sizeof(PacketInfo) * (*packet_count));

    const char* protocols[] = {"TCP", "UDP", "ICMP", "HTTP", "HTTPS"};
    const char* payloads[] = {
        "GET / HTTP/1.1",
        "Host: example.com",
        "User-Agent: ping-status/" VERSION,
        "Accept: text/html",
        "Connection: close",
        "SYN",
        "ACK",
        "FIN",
        "DATA"
    };

    for (int i = 0; i < *packet_count; i++) {
        snprintf(packets[i].source_ip, sizeof(packets[i].source_ip), "192.168.1.%d", 100 + rand() % 50);
        snprintf(packets[i].dest_ip, sizeof(packets[i].dest_ip), "10.0.0.%d", rand() % 255);
        packets[i].source_port = 50000 + rand() % 15000;
        packets[i].dest_port = rand() % 1000 + 1;
        snprintf(packets[i].protocol, sizeof(packets[i].protocol), "%s", protocols[rand() % 5]);
        packets[i].packet_size = 64 + rand() % 1400;
        packets[i].timestamp = time(NULL) + (rand() % 100) / 1000.0;

        // 生成负载
        if (rand() % 2 == 0) {
            snprintf(packets[i].payload, sizeof(packets[i].payload), "%s", payloads[rand() % 9]);
        } else {
            snprintf(packets[i].payload, sizeof(packets[i].payload), "DATA[%d]", rand() % 1000);
        }
    }

    return packets;
}

// HTTP时序测量（带数据包追踪）
HTTPTiming* measure_http_timing(const char* url) {
    HTTPTiming* timing = safe_malloc(sizeof(HTTPTiming));

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

// 内存管理
void free_http_timing(HTTPTiming* timing) {
    if (timing) {
        free(timing->effective_url);
        free(timing->content_type);
        if (timing->packets) {
            free(timing->packets);
        }
        free(timing);
    }
}

TimingHistory* create_timing_history(int initial_capacity) {
    TimingHistory* history = safe_malloc(sizeof(TimingHistory));

    history->timings = safe_malloc(sizeof(HTTPTiming*) * initial_capacity);
    history->count = 0;
    history->capacity = initial_capacity;

    return history;
}

void add_timing_to_history(TimingHistory* history, HTTPTiming* timing) {
    if (!history || !timing) return;

    if (history->count >= history->capacity) {
        history->capacity *= 2;
        HTTPTiming** new_timings = realloc(history->timings, sizeof(HTTPTiming*) * history->capacity);
        if (!new_timings) {
            fprintf(stderr, "内存重新分配失败\n");
            return;
        }
        history->timings = new_timings;
    }

    history->timings[history->count++] = timing;
}

void free_timing_history(TimingHistory* history) {
    if (history) {
        if (history->timings) {
            for (int i = 0; i < history->count; i++) {
                if (history->timings[i]) {
                    free_http_timing(history->timings[i]);
                }
            }
            free(history->timings);
        }
        free(history);
    }
}