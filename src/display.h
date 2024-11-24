#ifndef DISPLAY_H
#define DISPLAY_H

#include "dir.h"

enum DISPLAY_MARGIN {
	DISPLAY_M_PATH = 1,
	DISPLAY_M_CURS = 2,
	DISPLAY_M_MARK = 2,
	DISPLAY_M_LIST = 4,
};

/* initialize variables and setups the ncurses screen...  */
void screen_setup(void);

/* initialize and display the main display and preview display */
void init_displays(display_t *main_display, display_t *preview_display);

void draw_path(void);

/* display files from the main display */
void display_m(display_t dir_display, int cursor);

/* display files from the preview display */
void display_p(display_t *main_display, display_t *preview_display, int cursor);

void display_f(int cursor, int ammount);
#endif /* DISPLAY_H */
