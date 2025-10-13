/**
 * Ping-Status - 用户界面模块头文件
 */

#ifndef UI_H
#define UI_H

#include "types.h"

// UI显示函数
void print_beautiful_ui(HTTPTiming* timing);
void print_tree_view(HTTPTiming* timing, TimingHistory* history);
void print_compact_view(HTTPTiming* timing);

// 颜色和格式化函数
void set_color(const char* color);
void reset_color();
void print_header();
void print_footer();

// 进度条和图表函数
void print_progress_bar(double percentage, int width);
void print_timing_chart(HTTPTiming* timing);

#endif