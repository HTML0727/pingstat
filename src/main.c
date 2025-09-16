#include "logic.h"
#include "ui.h"
#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    app_state_t st;
    logic_init(&st);
    char config_path[512] = "";
    for (int i=1;i<argc;i++) {
        if (strcmp(argv[i], "--config")==0 && i+1<argc) strncpy(config_path, argv[++i], sizeof(config_path)-1);
        else if (strcmp(argv[i], "--export-json")==0) st.export_json = 1;
        else if (strcmp(argv[i], "--help")==0) { printf("Usage: pingstat [--config path] [--export-json]\n"); return 0; }
    }
    char urls[16][512]; int ucount=0;
    if (config_path[0]) config_load(config_path, urls, &ucount, &st.interval_ms, st.theme);
    else { char def[512]; snprintf(def, sizeof(def), "%s/.config/pingstat/config.json", getenv("HOME")?getenv("HOME"):"."); config_load(def, urls, &ucount, &st.interval_ms, st.theme); }
    if (ucount==0) { strncpy(st.targets[0].url, "https://example.com", sizeof(st.targets[0].url)-1); st.target_count = 1; }
    else { for (int i=0;i<ucount && i<MAX_TARGETS;i++) strncpy(st.targets[i].url, urls[i], sizeof(st.targets[i].url)-1); st.target_count = ucount; }
    char logp[512]; snprintf(logp, sizeof(logp), "%s/.local/share/pingstat/log.txt", getenv("HOME")?getenv("HOME"):"."); strncpy(st.log_path, logp, sizeof(st.log_path)-1);
    logger_init(st.log_path);
    logic_start_workers(&st);
    ui_start(&st);
    logic_stop_workers(&st);
    logger_close();
    if (st.export_json) logic_export_json(&st, "./pingstat-export.json");
    return 0;
}
