#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <libconfig.h>
#include "../include/config.h"
#include "../include/logic.h"

int create_config_directory() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    
    // 构建配置目录路径
    char config_dir[256];
    snprintf(config_dir, sizeof(config_dir), "%s/.config/ping-status", homedir);
    
    // 创建配置目录
    if (mkdir(config_dir, 0755) == -1) {
        if (errno != EEXIST) {
            perror("Error creating config directory");
            return 0;
        }
    }
    
    // 创建默认配置文件
    char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/config.conf", config_dir);
    
    FILE *config_file = fopen(config_path, "w");
    if (config_file) {
        fprintf(config_file, "# Ping-Status Configuration File\n");
        fprintf(config_file, "# This file controls the behavior of the ping-status application\n\n");
        
        fprintf(config_file, "[general]\n");
        fprintf(config_file, "# Default URL to check when none is provided\n");
        fprintf(config_file, "# default_url = https://example.com\n\n");
        
        fprintf(config_file, "# Default check interval in seconds (for continuous mode)\n");
        fprintf(config_file, "interval = 5\n\n");
        
        fprintf(config_file, "# Default number of requests to make\n");
        fprintf(config_file, "count = 1\n\n");
        
        fprintf(config_file, "# Default output format (text, json)\n");
        fprintf(config_file, "output_format = text\n\n");
        
        fprintf(config_file, "# Default theme (tokyo-night, adiwata, apple-system, custom)\n");
        fprintf(config_file, "theme = tokyo-night\n\n");
        
        fprintf(config_file, "# Whether to verify SSL certificates (0 = no, 1 = yes)\n");
        fprintf(config_file, "verify_ssl = 1\n\n");
        
        fprintf(config_file, "# Timeout for HTTP requests in seconds\n");
        fprintf(config_file, "timeout = 10\n\n");
        
        fprintf(config_file, "# User agent string to use for requests\n");
        fprintf(config_file, "user_agent = ping-status/1.0\n\n");
        
        fprintf(config_file, "[display]\n");
        fprintf(config_file, "# Refresh rate for the UI in milliseconds\n");
        fprintf(config_file, "refresh_rate = 100\n\n");
        
        fprintf(config_file, "# Whether to show progress bars\n");
        fprintf(config_file, "show_progress_bars = 1\n\n");
        
        fprintf(config_file, "# Whether to show history chart\n");
        fprintf(config_file, "show_history = 1\n\n");
        
        fprintf(config_file, "# Maximum number of history entries to keep\n");
        fprintf(config_file, "max_history_entries = 50\n\n");
        
        fprintf(config_file, "# Whether to show detailed timing information\n");
        fprintf(config_file, "show_detailed_timing = 1\n\n");
        
        fprintf(config_file, "[behavior]\n");
        fprintf(config_file, "# Whether to follow redirects\n");
        fprintf(config_file, "follow_redirects = 1\n\n");
        
        fprintf(config_file, "# Maximum number of redirects to follow\n");
        fprintf(config_file, "max_redirects = 10\n\n");
        
        fprintf(config_file, "# Whether to include response body in timing (0 = headers only, 1 = full response)\n");
        fprintf(config_file, "include_body = 1\n\n");
        
        fprintf(config_file, "# Whether to resolve DNS for display\n");
        fprintf(config_file, "resolve_dns = 1\n");
        
        fclose(config_file);
    }
    
    // 创建默认CSS配置文件
    char css_path[256];
    snprintf(css_path, sizeof(css_path), "%s/css.conf", config_dir);
    
    FILE *css_file = fopen(css_path, "w");
    if (css_file) {
        fprintf(css_file, "/* Ping-Status Theme Configuration */\n");
        fprintf(css_file, "/* This file controls the visual appearance of ping-status */\n\n");
        
        fprintf(css_file, ":root {\n");
        fprintf(css_file, "  /* Base colors - used by all themes unless overridden */\n");
        fprintf(css_file, "  --primary-color: #1a1b26;\n");
        fprintf(css_file, "  --secondary-color: #24283b;\n");
        fprintf(css_file, "  --accent-color: #bb9af7;\n");
        fprintf(css_file, "  --background-color: #16161e;\n");
        fprintf(css_file, "  --text-color: #a9b1d6;\n");
        fprintf(css_file, "  --success-color: #73daca;\n");
        fprintf(css_file, "  --warning-color: #e0af68;\n");
        fprintf(css_file, "  --error-color: #f7768e;\n");
        fprintf(css_file, "  --border-color: #414868;\n");
        fprintf(css_file, "  --highlight-color: #7aa2f7;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, "/* Tokyo Night Theme (default) */\n");
        fprintf(css_file, ".tokyo-night {\n");
        fprintf(css_file, "  --primary-color: #1a1b26;\n");
        fprintf(css_file, "  --secondary-color: #24283b;\n");
        fprintf(css_file, "  --accent-color: #bb9af7;\n");
        fprintf(css_file, "  --background-color: #16161e;\n");
        fprintf(css_file, "  --text-color: #a9b1d6;\n");
        fprintf(css_file, "  --success-color: #73daca;\n");
        fprintf(css_file, "  --warning-color: #e0af68;\n");
        fprintf(css_file, "  --error-color: #f7768e;\n");
        fprintf(css_file, "  --border-color: #414868;\n");
        fprintf(css_file, "  --highlight-color: #7aa2f7;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, "/* Adiwata Theme */\n");
        fprintf(css_file, ".adiwata {\n");
        fprintf(css_file, "  --primary-color: #2a2b36;\n");
        fprintf(css_file, "  --secondary-color: #343746;\n");
        fprintf(css_file, "  --accent-color: #6d8cbd;\n");
        fprintf(css_file, "  --background-color: #22232e;\n");
        fprintf(css_file, "  --text-color: #c5c8d1;\n");
        fprintf(css_file, "  --success-color: #8dc776;\n");
        fprintf(css_file, "  --warning-color: #e6b455;\n");
        fprintf(css_file, "  --error-color: #e35555;\n");
        fprintf(css_file, "  --border-color: #44475a;\n");
        fprintf(css_file, "  --highlight-color: #5e81ac;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, "/* Apple System Theme */\n");
        fprintf(css_file, ".apple-system {\n");
        fprintf(css_file, "  --primary-color: #ffffff;\n");
        fprintf(css_file, "  --secondary-color: #f5f5f7;\n");
        fprintf(css_file, "  --accent-color: #007aff;\n");
        fprintf(css_file, "  --background-color: #ffffff;\n");
        fprintf(css_file, "  --text-color: #333333;\n");
        fprintf(css_file, "  --success-color: #34c759;\n");
        fprintf(css_file, "  --warning-color: #ff9500;\n");
        fprintf(css_file, "  --error-color: #ff3b30;\n");
        fprintf(css_file, "  --border-color: #c6c6c8;\n");
        fprintf(css_file, "  --highlight-color: #007aff;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, "/* Custom Theme - uncomment and modify to create your own theme */\n");
        fprintf(css_file, "/*\n");
        fprintf(css_file, ".custom-theme {\n");
        fprintf(css_file, "  --primary-color: #your-color;\n");
        fprintf(css_file, "  --secondary-color: #your-color;\n");
        fprintf(css_file, "  --accent-color: #your-color;\n");
        fprintf(css_file, "  --background-color: #your-color;\n");
        fprintf(css_file, "  --text-color: #your-color;\n");
        fprintf(css_file, "  --success-color: #your-color;\n");
        fprintf(css_file, "  --warning-color: #your-color;\n");
        fprintf(css_file, "  --error-color: #your-color;\n");
        fprintf(css_file, "  --border-color: #your-color;\n");
        fprintf(css_file, "  --highlight-color: #your-color;\n");
        fprintf(css_file, "}\n");
        fprintf(css_file, "*/\n\n");
        
        fprintf(css_file, "/* UI Elements */\n");
        fprintf(css_file, ".status-container {\n");
        fprintf(css_file, "  background-color: var(--background-color);\n");
        fprintf(css_file, "  color: var(--text-color);\n");
        fprintf(css_file, "  border: 1px solid var(--border-color);\n");
        fprintf(css_file, "  border-radius: 8px;\n");
        fprintf(css_file, "  padding: 1rem;\n");
        fprintf(css_file, "  margin: 0.5rem;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".status-header {\n");
        fprintf(css_file, "  color: var(--accent-color);\n");
        fprintf(css_file, "  border-bottom: 1px solid var(--border-color);\n");
        fprintf(css_file, "  padding-bottom: 0.5rem;\n");
        fprintf(css_file, "  margin-bottom: 1rem;\n");
        fprintf(css_file, "  font-weight: bold;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".timing-item {\n");
        fprintf(css_file, "  display: flex;\n");
        fprintf(css_file, "  justify-content: space-between;\n");
        fprintf(css_file, "  padding: 0.25rem 0;\n");
        fprintf(css_file, "  border-bottom: 1px solid var(--secondary-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".timing-value {\n");
        fprintf(css_file, "  font-weight: bold;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".timing-value.good {\n");
        fprintf(css_file, "  color: var(--success-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".timing-value.warning {\n");
        fprintf(css_file, "  color: var(--warning-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".timing-value.bad {\n");
        fprintf(css_file, "  color: var(--error-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".progress-bar {\n");
        fprintf(css_file, "  height: 8px;\n");
        fprintf(css_file, "  background-color: var(--secondary-color);\n");
        fprintf(css_file, "  border-radius: 4px;\n");
        fprintf(css_file, "  overflow: hidden;\n");
        fprintf(css_file, "  margin-top: 0.25rem;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".progress-fill {\n");
        fprintf(css_file, "  height: 100%;\n");
        fprintf(css_file, "  background-color: var(--accent-color);\n");
        fprintf(css_file, "  transition: width 0.3s ease;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".progress-fill.good {\n");
        fprintf(css_file, "  background-color: var(--success-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".progress-fill.warning {\n");
        fprintf(css_file, "  background-color: var(--warning-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".progress-fill.bad {\n");
        fprintf(css_file, "  background-color: var(--error-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".http-status {\n");
        fprintf(css_file, "  display: inline-block;\n");
        fprintf(css_file, "  padding: 0.25rem 0.5rem;\n");
        fprintf(css_file, "  border-radius: 4px;\n");
        fprintf(css_file, "  font-weight: bold;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".http-status-2xx {\n");
        fprintf(css_file, "  background-color: var(--success-color);\n");
        fprintf(css_file, "  color: var(--background-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".http-status-3xx {\n");
        fprintf(css_file, "  background-color: var(--warning-color);\n");
        fprintf(css_file, "  color: var(--background-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".http-status-4xx, .http-status-5xx {\n");
        fprintf(css_file, "  background-color: var(--error-color);\n");
        fprintf(css_file, "  color: var(--background-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".history-chart {\n");
        fprintf(css_file, "  height: 60px;\n");
        fprintf(css_file, "  background-color: var(--secondary-color);\n");
        fprintf(css_file, "  border-radius: 4px;\n");
        fprintf(css_file, "  display: flex;\n");
        fprintf(css_file, "  align-items: flex-end;\n");
        fprintf(css_file, "  padding: 0.25rem;\n");
        fprintf(css_file, "  gap: 2px;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".history-bar {\n");
        fprintf(css_file, "  background-color: var(--accent-color);\n");
        fprintf(css_file, "  border-radius: 2px 2px 0 0;\n");
        fprintf(css_file, "  flex: 1;\n");
        fprintf(css_file, "  min-width: 2px;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".history-bar.good {\n");
        fprintf(css_file, "  background-color: var(--success-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".history-bar.warning {\n");
        fprintf(css_file, "  background-color: var(--warning-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".history-bar.bad {\n");
        fprintf(css_file, "  background-color: var(--error-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".control-button {\n");
        fprintf(css_file, "  background-color: var(--secondary-color);\n");
        fprintf(css_file, "  border: 1px solid var(--border-color);\n");
        fprintf(css_file, "  color: var(--text-color);\n");
        fprintf(css_file, "  padding: 0.5rem 1rem;\n");
        fprintf(css_file, "  border-radius: 4px;\n");
        fprintf(css_file, "  cursor: pointer;\n");
        fprintf(css_file, "  margin-right: 0.5rem;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".control-button:hover {\n");
        fprintf(css_file, "  background-color: var(--accent-color);\n");
        fprintf(css_file, "  color: var(--background-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".theme-button {\n");
        fprintf(css_file, "  width: 20px;\n");
        fprintf(css_file, "  height: 20px;\n");
        fprintf(css_file, "  border-radius: 50%;\n");
        fprintf(css_file, "  border: 2px solid var(--border-color);\n");
        fprintf(css_file, "  cursor: pointer;\n");
        fprintf(css_file, "  display: inline-block;\n");
        fprintf(css_file, "  margin-right: 0.5rem;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".theme-button.active {\n");
        fprintf(css_file, "  border-color: var(--accent-color);\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".theme-button.tokyo-night {\n");
        fprintf(css_file, "  background-color: #bb9af7;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".theme-button.adiwata {\n");
        fprintf(css_file, "  background-color: #6d8cbd;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".theme-button.apple-system {\n");
        fprintf(css_file, "  background-color: #007aff;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".loading {\n");
        fprintf(css_file, "  opacity: 0.7;\n");
        fprintf(css_file, "  pointer-events: none;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, ".loading::after {\n");
        fprintf(css_file, "  content: '';\n");
        fprintf(css_file, "  display: inline-block;\n");
        fprintf(css_file, "  width: 12px;\n");
        fprintf(css_file, "  height: 12px;\n");
        fprintf(css_file, "  border: 2px solid transparent;\n");
        fprintf(css_file, "  border-top: 2px solid var(--accent-color);\n");
        fprintf(css_file, "  border-radius: 50%;\n");
        fprintf(css_file, "  animation: spin 1s linear infinite;\n");
        fprintf(css_file, "  margin-left: 8px;\n");
        fprintf(css_file, "}\n\n");
        
        fprintf(css_file, "@keyframes spin {\n");
        fprintf(css_file, "  0%% { transform: rotate(0deg); }\n");
        fprintf(css_file, "  100%% { transform: rotate(360deg); }\n");
        fprintf(css_file, "}\n");
                
        fclose(css_file);
    }
    
    return 1;
}

void read_config_file(AppConfig* config) {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    
    char config_path[256];
    snprintf(config_path, sizeof(config_path), "%s/.config/ping-status/config.conf", homedir);
    
    config_t cfg;
    config_init(&cfg);
    
    if (config_read_file(&cfg, config_path) == CONFIG_TRUE) {
        const char* theme;
        if (config_lookup_string(&cfg, "general.theme", &theme) == CONFIG_TRUE) {
            free(config->theme);
            config->theme = strdup(theme);
        }
        
        int interval;
        if (config_lookup_int(&cfg, "general.interval", &interval) == CONFIG_TRUE) {
            config->interval = interval;
        }
        
        int count;
        if (config_lookup_int(&cfg, "general.count", &count) == CONFIG_TRUE) {
            config->count = count;
        }
        
        const char* output_format;
        if (config_lookup_string(&cfg, "general.output_format", &output_format) == CONFIG_TRUE) {
            free(config->output_format);
            config->output_format = strdup(output_format);
        }
        
        int verify_ssl;
        if (config_lookup_int(&cfg, "general.verify_ssl", &verify_ssl) == CONFIG_TRUE) {
            // 这里可以设置cURL的SSL验证选项
        }
        
        int timeout;
        if (config_lookup_int(&cfg, "general.timeout", &timeout) == CONFIG_TRUE) {
            // 这里可以设置cURL的超时选项
        }
        
        const char* user_agent;
        if (config_lookup_string(&cfg, "general.user_agent", &user_agent) == CONFIG_TRUE) {
            // 这里可以设置cURL的User-Agent
        }
    }
    
    config_destroy(&cfg);
}
