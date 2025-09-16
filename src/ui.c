#include "ui.h"
#include "theme.h"
#include "logger.h"
#include "logic.h"
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static app_state_t *gstate;
static int running = 0;

static void draw_left(WINDOW *w) {
    int y=1;
    mvwprintw(w,y++,1,"Targets:"); 
    for (int i=0;i<gstate->target_count;i++) {
        target_t *t = &gstate->targets[i];
        if (i == gstate->selected) wattron(w, A_REVERSE);
        mvwprintw(w,y++,2,"%d) %s", i+1, t->url);
        if (i == gstate->selected) wattroff(w, A_REVERSE);
    }
    y++;
    mvwprintw(w,y++,1,"Interval: %dms", gstate->interval_ms);
    mvwprintw(w,y++,1,"Theme: %s", gstate->theme);
}

static void draw_center(WINDOW *w) {
    int rows, cols; getmaxyx(w, rows, cols);
    target_t *t = &gstate->targets[gstate->selected];
    mvwprintw(w,1,1,"Current: %s", t->url);
    mvwprintw(w,2,1,"Code: %ld  bytes: %zu  last_ms: %.1f", t->last.response_code, t->last.bytes, t->last.total*1000.0);
    int h = rows - 6;
    int wcols = cols - 2;
    double maxv = 1.0;
    for (int i=0;i<HISTORY_LEN;i++) if (t->history[i] > maxv) maxv = t->history[i];
    for (int col=0; col < wcols && col < HISTORY_LEN; col++) {
        int idx = (t->hist_pos + col) % HISTORY_LEN;
        double v = t->history[idx];
        int bar = (int)((v / maxv) * h);
        for (int row=0; row<h; row++) {
            mvwprintw(w, rows-3-row, 1+col, (h-row)<=bar?"*":" ");
        }
    }
}

static void draw_right(WINDOW *w) {
    int rows, cols; getmaxyx(w, rows, cols);
    mvwprintw(w,1,1,"Logs:");
    FILE *f = fopen(gstate->log_path,"r");
    if (!f) { mvwprintw(w,2,1,"(no logs)"); return; }
    int maxlines = rows - 3;
    char **lines = malloc(sizeof(char*)*maxlines);
    for (int i=0;i<maxlines;i++) lines[i]=NULL;
    int count=0;
    char buf[1024];
    while(fgets(buf,sizeof(buf),f)) {
        if (count < maxlines) { lines[count] = strdup(buf); count++; }
        else { free(lines[0]); for (int j=1;j<maxlines;j++) lines[j-1]=lines[j]; lines[maxlines-1]=strdup(buf); }
    }
    fclose(f);
    int y=2;
    for (int i=0;i<count;i++) { mvwprintw(w,y++,1, "%s", lines[i]); free(lines[i]); }
    free(lines);
}

void ui_start(app_state_t *s) {
    gstate = s;
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE); curs_set(0);
    theme_load(s->theme);
    running = 1;
    while (running) {
        int rows, cols; getmaxyx(stdscr, rows, cols);
        int left_w = cols/4; int right_w = cols/3; int center_w = cols - left_w - right_w - 4;
        WINDOW *left = newwin(rows, left_w, 0, 0);
        WINDOW *center = newwin(rows, center_w, 0, left_w+2);
        WINDOW *right = newwin(rows, right_w, 0, left_w+2+center_w+2);
        werase(left); werase(center); werase(right);
        box(left, 0,0); box(center,0,0); box(right,0,0);
        draw_left(left); draw_center(center); draw_right(right);
        mvprintw(rows-1,1,"Selected %d/%d", gstate->selected+1, gstate->target_count);
        refresh(); wrefresh(left); wrefresh(center); wrefresh(right);
        int ch = getch();
        if (ch == 'q' || ch == 'Q') { gstate->running = 0; running = 0; break; }
        else if (ch == KEY_UP) { if (gstate->selected>0) gstate->selected--; }
        else if (ch == KEY_DOWN) { if (gstate->selected < gstate->target_count-1) gstate->selected++; }
        else if (ch == 't') { if (strcmp(gstate->theme, "tokyo-night")==0) strcpy(gstate->theme, "adiwata"); else if (strcmp(gstate->theme, "adiwata")==0) strcpy(gstate->theme, "apple-system-colors"); else strcpy(gstate->theme, "tokyo-night"); theme_load(gstate->theme); }
        else if (ch == 'e') { logic_export_json(gstate, "./pingstat-export.json"); logger_log("exported json to ./pingstat-export.json"); }
        delwin(left); delwin(center); delwin(right);
        usleep(100*1000);
    }
    endwin();
}

void ui_stop(void) {}
