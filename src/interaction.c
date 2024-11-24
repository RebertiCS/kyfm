#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../config.h"
#include "dir.h"
#include "display.h"
#include "interaction.h"

/*****************************************************************************/
/*	 The user input and the clipboard are hadled here		     */
/*****************************************************************************/

struct selection file_selection;

void handle_input(display_t *main_display, int *cursor, char key)
{
	switch (key) {
	case KEY_MOV_UP:
		if (*cursor > 0)
			*cursor -= 1;

		break;

	case KEY_MOV_DOWN:
		/* "cursor+1" is used to see where the cursor will be */
		if (*cursor+1 < main_display->files.size)
			*cursor += 1;

		break;

	case KEY_MOV_LEFT: {
		chdir((config.path[1] != '\0') ? "../" : "/");
		list_files(&main_display->files, NULL);

		*cursor = 0;

		draw_path();
		display_m(*main_display, *cursor);

		break;
	}
	case KEY_MOV_RIGHT:
		if (chdir(main_display->files.list[*cursor]) == 0) {
			list_files(&main_display->files, NULL);

			*cursor = 0;

			draw_path();
			display_m(*main_display, *cursor);
		}

		break;

	case KEY_MOV_TOP:
		*cursor = 0;
		break;

	case KEY_MOV_BOTTOM:
		*cursor = main_display->files.size-1;
		break;

	case KEY_FILE_OPEN:
		file_open(main_display->files.list[*cursor]);
		break;

	case KEY_FILE_MARK:
		main_display->files.marked[*cursor] =
			!main_display->files.marked[*cursor];

		draw_path();
		display_m(*main_display, *cursor);
		refresh();
		break;

	case KEY_FILE_HIDDEN:
		*cursor = 0;
		config.hidden = !config.hidden;

		list_files(&main_display->files, NULL);
		display_m(*main_display, *cursor);
		break;

	case KEY_FILE_DEL:
		if (sel_del(main_display) == 0)
			*cursor = 0;

		draw_path();
		list_files(&main_display->files, NULL);
		display_m(*main_display, *cursor);
		break;

	case KEY_FILE_YANK:
		sel_copy(main_display);
		file_selection.type = SEC_CLIP_YANK;
		break;

	case KEY_FILE_CUT:
		sel_copy(main_display);
		file_selection.type = SEC_CLIP_CUT;
		break;

	case KEY_FILE_PASTE:
		sel_paste();

		draw_path();
		list_files(&main_display->files, NULL);
		display_m(*main_display, *cursor);
		break;
	}
}

int ask_usr(char *text)
{
	move(config.size.y - 1, 0); // move to begining of line
	clrtoeol();		    // Clean displayed path

	mvprintw(config.size.y - 1, 1, text);
	refresh();

	char key = getchar();

	if (key != 'y')
		return -1; // user answear wans't "Yes"
	
	return 0;
}

void sel_copy(display_t *dir_display)
{
	file_selection.size = 0;

	for (size_t i = 0; i < dir_display->files.size; ++i) {
		if (dir_display->files.marked[i] == 1)
			sprintf(file_selection.files[file_selection.size++],
				"%s/%s", config.path, dir_display->files.list[i]);
	}
}

void sel_paste(void)
{
	for (size_t i = 0; i < file_selection.size; ++i) {
		if (fork() == 0) {
			if (file_selection.type & SEC_CLIP_YANK)
				execlp("cp", "cp", "-r",
				       file_selection.files[i], config.path,
				       (char *)NULL);
			else
				execlp("mv", "mv", file_selection.files[i],
				       config.path, (char *)NULL);
		}
		wait(NULL);
	}
}

int sel_del(display_t *dir_display)
{
	if(ask_usr("Are you sure you want to delete this files?(y/n)") != 0)
		return -1;

	sel_copy(dir_display);

	for (size_t i = 0; i < file_selection.size; ++i)
		remove(file_selection.files[i]);

	return 0;
}
