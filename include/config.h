#ifndef CONFIG_H
#define CONFIG_H
int config_load(const char *path, char urls[][512], int *count, int *interval_ms, char *theme);
#endif
