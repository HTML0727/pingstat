#include "network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static size_t sink(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t n = size * nmemb;
    size_t *bytes = (size_t *)userdata;
    *bytes += n;
    return n;
}

int http_probe_once(const char *url, net_timings_t *out) {
    CURL *curl = curl_easy_init();
    if (!curl) return -1;
    memset(out,0,sizeof(*out));
    size_t bytes = 0;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sink);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bytes);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "pingstat/0.2");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        out->success = 0;
        out->bytes = bytes;
        curl_easy_cleanup(curl);
        return (int)res;
    }
    out->success = 1;
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &out->namelookup);
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &out->connect);
    curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &out->appconnect);
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &out->pretransfer);
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &out->starttransfer);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &out->total);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &out->response_code);
    out->bytes = bytes;
    curl_easy_cleanup(curl);
    return 0;
}
