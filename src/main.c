#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../config.h"
#include "dir.h"
#include "display.h"
#include "interaction.h"

struct config config;

int main(void)
{
	screen_setup();

	display_t main_display, preview_display;

	init_displays(&main_display, &preview_display);

	int cursor = 0;
	int main_redraw = 0;

	move(cursor, DISPLAY_M_LIST);

	for (char key = getch(); key != KEY_QUIT; key = getch()) {
		handle_input(&main_display, &cursor, key);
		display_p(&main_display, &preview_display, cursor);

		// If terminal size has changed, update the window size
		if (is_term_resized(config.size.y, config.size.x) == 1) {
			getmaxyx(stdscr, config.size.y, config.size.x);

			delwin(main_display.screen);
			delwin(preview_display.screen);

			init_displays(&main_display, &preview_display);
		}

		display_f(cursor, main_display.files.size);
		move(cursor, DISPLAY_M_LIST);

		/**************************************************************/
		/*     The window shall be redrawed after the cursor value    */
		/* surpasses the screen size, after that the window will only */
		/* stop to redraw after the cursor hits the top of the window.*/
		/**************************************************************/
		if (cursor >= config.size.y-DISPLAY_M_CURS || main_redraw == 1) {
			display_m(main_display, cursor);
			main_redraw = 1;

			if (cursor == 0)
				main_redraw = 0;
		}
	}

	if (main_display.files.mem_count > 0) {
		for (size_t i = 0; i < main_display.files.mem_alloc; ++i)
			free(main_display.files.list[i]);

		free(main_display.files.list);
		free(main_display.files.marked);
	}

	if (preview_display.files.mem_count > 0) {
		for (size_t i = 0; i < preview_display.files.mem_alloc; ++i)
			free(preview_display.files.list[i]);

		free(preview_display.files.list);
		free(preview_display.files.marked);
	}

	endwin();
	delwin(main_display.screen);
	delwin(preview_display.screen);

	return 0;
}
