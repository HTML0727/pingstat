/**
 * Ping-Status - 主程序入口
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "../include/types.h"
#include "../include/network.h"
#include "../include/ui.h"
#include "../include/config.h"
#include "../include/utils.h"

// 全局状态
UIState ui_state;
volatile sig_atomic_t stop_requested = 0;

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

    // 初始化UI状态
    snprintf(ui_state.url, sizeof(ui_state.url), "%s", config->url);
    snprintf(ui_state.status, sizeof(ui_state.status), "就绪");
    ui_state.is_measuring = false;
    ui_state.progress = 0;

    printf("🚀 Ping-Status %s - 开始监控 %s\n", VERSION, config->url);
    if (config->verbose) {
        printf("配置: 间隔=%d秒, 计数=%d, 连续=%s\n",
               config->interval, config->count, config->continuous ? "是" : "否");
    }

    int count = 0;
    while (!stop_requested && (config->continuous || count < config->count)) {
        if (config->verbose) {
            printf("\n--- 第 %d 次测量 ---\n", count + 1);
        }

        snprintf(ui_state.status, sizeof(ui_state.status), "测量中... (%d/%d)",
                count + 1, config->continuous ? -1 : config->count);
        ui_state.is_measuring = true;

        // 执行测量
        HTTPTiming* timing = measure_http_timing(config->url);

        if (timing) {
            // 输出结果
            if (config->json_output) {
                print_timing_json(timing);
            } else if (config->tree_output) {
                printf("🌳 树形图显示模式\n");
                printf("═══════════════════════════════════════\n\n");
                print_tree_view(timing, NULL);
            } else {
                print_beautiful_ui(timing);
            }

            free_http_timing(timing);
        } else {
            snprintf(ui_state.status, sizeof(ui_state.status), "测量失败");
            if (!config->json_output) {
                printf("❌ 测量失败\n");
            }
        }

        ui_state.is_measuring = false;
        count++;

        // 等待间隔（除非是最后一次请求）
        if (!stop_requested && (config->continuous || count < config->count)) {
            for (int i = 0; i < config->interval && !stop_requested; i++) {
                sleep(1);
            }
        }
    }

    // 显示最终统计
    if (!config->json_output) {
        printf("\n✅ 测量完成! 总请求数: %d\n", count);
    }

    free_config(config);
    return EXIT_SUCCESS;
}