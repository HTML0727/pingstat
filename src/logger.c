#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(p,m) _mkdir(p)
#else
#include <sys/stat.h>
#endif

static FILE *logf = NULL;
static char logpath[512];

void logger_init(const char *path) {
    if (!path) return;
    strncpy(logpath, path, sizeof(logpath)-1);
    char dir[512];
    strcpy(dir, path);
    char *p = strrchr(dir, '/');
    if (p) { *p = '\0'; mkdir(dir, 0755); }
    logf = fopen(path, "a");
    if (!logf) {
        perror("logger open");
    }
}

void logger_log(const char *fmt, ...) {
    if (!logf) return;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(logf, "%s ", ts);
    va_list ap; va_start(ap, fmt);
    vfprintf(logf, fmt, ap);
    va_end(ap);
    fprintf(logf, "\n");
    fflush(logf);
}

void logger_close(void) {
    if (logf) fclose(logf);
    logf = NULL;
}
