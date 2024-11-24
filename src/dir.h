#ifndef DIR_H
#define DIR_H

#include <dirent.h>
#include <ncurses.h>
#include <stddef.h>

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define FILE_LIST_SZ 100

struct directory_display {
	WINDOW *screen;

	struct files {
		size_t size;      /* total ammount of files in a dir */
		size_t mem_count; /* memory allocated for the array */
		size_t mem_alloc; /* memory allocated for the files names */

		short *marked;         /* files marked to be used in selection */
		char **list;           /* where the file list is stored */
		char   dir[MAXNAMLEN]; /* used to fix the files display colors */
	} files;

	/* position of the screen in the terminal window */
	struct position {
		int x[2];
		int y[2];
	} position;
};

typedef struct directory_display display_t;

int is_file(char *path);

/* get information from a directory and store it in files */
int list_files(struct files *files, char *path);

/* call externel programs to read file contents*/
int file_open(char *file_name);
#endif /* DIR_H */
