#ifndef CONFIG_HEADER_FILE
#define CONFIG_HEADER_FILE

#include <dirent.h>
#include "./src/dir.h"

struct config {
	short int hidden; /* 1 = show hidden files */
	char path[PATH_MAX];

	struct envp {
		/* used to open files */
		char *defaults[4];

		/* used to display the path */
		char *user_name;
		char *home_name;
		char  host_name[1024];
	} envp;

	/* terminal screen size */
	struct size {
		size_t x;
		size_t y;
	} size;
};

extern struct config config;

#define KEY_QUIT	'q'
#define KEY_MOV_UP	'k'
#define KEY_MOV_DOWN	'j'
#define KEY_MOV_LEFT	'h'
#define KEY_MOV_RIGHT	'l'
#define KEY_MOV_TOP	'g'
#define KEY_MOV_BOTTOM	'G'

#define KEY_FILE_MARK	' '
#define KEY_FILE_DEL	'D'
#define KEY_FILE_CUT	'd'
#define KEY_FILE_YANK	'y'
#define KEY_FILE_PASTE	'p'
#define KEY_FILE_OPEN	'o'
#define KEY_FILE_HIDDEN	'z'

#endif
