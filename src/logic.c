#include "logic.h"
#include "logger.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

static pthread_t workers[MAX_TARGETS];
static int worker_running[MAX_TARGETS];
static app_state_t *gstate;

static void *worker_fn(void *arg) {
    int idx = (int)(long)arg;
    while (worker_running[idx]) {
        target_t *t = &gstate->targets[idx];
        net_timings_t nt;
        int r = http_probe_once(t->url, &nt);
        if (r==0) {
            t->last = nt;
            t->history[t->hist_pos] = nt.total*1000.0;
            t->hist_pos = (t->hist_pos + 1) % HISTORY_LEN;
            logger_log("%s %ld %.1fms bytes=%zu", t->url, nt.response_code, nt.total*1000.0, nt.bytes);
        } else {
            t->last.success = 0;
            t->history[t->hist_pos] = 0.0;
            t->hist_pos = (t->hist_pos + 1) % HISTORY_LEN;
            logger_log("%s error=%d", t->url, r);
        }
        int sleep_ms = gstate->interval_ms;
        int steps = sleep_ms / 200;
        for (int i=0;i<steps && worker_running[idx];i++) usleep(200*1000);
    }
    return NULL;
}

void logic_init(app_state_t *s) {
    memset(s,0,sizeof(*s));
    s->interval_ms = 2000;
    s->running = 1;
    s->selected = 0;
    strncpy(s->theme, "tokyo-night", sizeof(s->theme)-1);
    snprintf(s->log_path, sizeof(s->log_path)-1, "%s/.local/share/pingstat/log.txt", getenv("HOME")?getenv("HOME"):".");
    s->export_json = 0;
    gstate = s;
    for (int i=0;i<MAX_TARGETS;i++) worker_running[i]=0;
}

void logic_start_workers(app_state_t *s) {
    for (int i=0;i<s->target_count;i++) {
        worker_running[i] = 1;
        pthread_create(&workers[i], NULL, worker_fn, (void*)(long)i);
    }
}

void logic_stop_workers(app_state_t *s) {
    for (int i=0;i<s->target_count;i++) {
        worker_running[i] = 0;
    }
    for (int i=0;i<s->target_count;i++) {
        pthread_join(workers[i], NULL);
    }
}

void logic_export_json(app_state_t *s, const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    fprintf(f, "{\n  \"targets\": [\n" );
    for (int i=0;i<s->target_count;i++) {
        target_t *t = &s->targets[i];
        fprintf(f, "    {\"url\": \"%s\", \"last_ms\": %.1f, \"code\": %ld}", t->url, t->last.total*1000.0, t->last.response_code);
        if (i < s->target_count-1) fprintf(f, ",\n");
        else fprintf(f, "\n");
    }
    fprintf(f, "  ]\n}\n");
    fclose(f);
}
