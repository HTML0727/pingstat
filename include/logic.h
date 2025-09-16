#ifndef LOGIC_H
#define LOGIC_H
#include "network.h"
#define MAX_TARGETS 16
#define HISTORY_LEN 128

typedef struct {
    char url[512];
    net_timings_t last;
    double history[HISTORY_LEN];
    int hist_pos;
} target_t;

typedef struct {
    target_t targets[MAX_TARGETS];
    int target_count;
    int interval_ms;
    int running;
    int selected;
    char theme[64];
    char log_path[512];
    int export_json;
} app_state_t;

void logic_init(app_state_t *s);
void logic_start_workers(app_state_t *s);
void logic_stop_workers(app_state_t *s);
void logic_export_json(app_state_t *s, const char *path);
#endif
