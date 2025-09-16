#ifndef NETWORK_H
#define NETWORK_H
#include <curl/curl.h>
#include <stddef.h>

typedef struct {
    double namelookup;
    double connect;
    double appconnect;
    double pretransfer;
    double starttransfer;
    double total;
    long response_code;
    size_t bytes;
    int success;
} net_timings_t;

int http_probe_once(const char *url, net_timings_t *out);
#endif
