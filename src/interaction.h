#ifndef INTERACTION_H
#define INTERACTION_H

#include <stddef.h>

#include "display.h"
#include "dir.h"

enum selection_type {
	SEC_CLIP_YANK = 1,
	SEC_CLIP_CUT  = 2,
	SEC_FILE_DEL  = 4,
};

/* The program clippboard */
struct selection {
	int    type; /* type of opperation to be executed */
	size_t size; /* ammount of files selected */

	char   files[100][MAXNAMLEN]; /* list of files names */
};

extern struct selection file_selection;

void handle_input(display_t *main_display, int *cursor, char key);

int ask_usr(char *text);

/* copy marked files from dir_display to the selection */
void sel_copy(display_t *dir_display);

/* copy/move files from selection to the cwd */
void sel_paste(void);

/* delete files from selection */
int sel_del(display_t *dir_display);

#endif /* INTERACTION_H */
