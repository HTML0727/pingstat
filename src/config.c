#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int config_load(const char *path, char urls[][512], int *count, int *interval_ms, char *theme) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    fseek(f,0,SEEK_END);
    long sz = ftell(f);
    fseek(f,0,SEEK_SET);
    char *buf = malloc(sz+1);
    fread(buf,1,sz,f);
    buf[sz]='\0';
    fclose(f);
    char *p = strstr(buf, "\"urls\"");
    if (p) {
        p = strchr(p, '[');
        if (p) {
            p++;
            while(*p && *count < 16) {
                char *q = strchr(p, '"');
                if (!q) break;
                q++;
                char *r = strchr(q, '"');
                if (!r) break;
                int len = r - q;
                if (len>0 && len<512) {
                    strncpy(urls[*count], q, len);
                    urls[*count][len] = '\0';
                    (*count)++ ;
                }
                p = r+1;
            }
        }
    }
    p = strstr(buf, "\"interval_ms\"");
    if (p) {
        p = strchr(p, ':');
        if (p) *interval_ms = atoi(p+1);
    }
    p = strstr(buf, "\"theme\"");
    if (p) {
        p = strchr(p, '"');
        if (p) {
            p = strchr(p+1, '"');
            if (p) {
                char *q = p+1;
                char *r = strchr(q,'"');
                if (r) {
                    int len = r-q;
                    if (len>0) {
                        strncpy(theme, q, len);
                        theme[len]='\0';
                    }
                }
            }
        }
    }
    free(buf);
    return 0;
}
