#include "theme.h"
#include <string.h>
#include <ncurses.h>

int theme_load(const char *name) {
    start_color();
    if (strcmp(name, "tokyo-night")==0) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
    } else if (strcmp(name, "adiwata")==0) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
    } else {
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
    }
    return 0;
}

const char* theme_list(void){
    return "tokyo-night,adiwata,apple-system-colors";
}
