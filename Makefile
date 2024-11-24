TARGET_EXEC = kyfm

CC 	= gcc
CLIBS 	= -lncurses
CFLAGS 	= -O3
DEBUG_CFLAGS = -Wall -Werror -pedantic -ggdb3 -Wno-error=unknown-pragmas

SRC_DIRS 	= ./src
BUILD_DIR 	= ./build

SRCS = $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

# Program binary
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(CLIBS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
ifdef DEBUG
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@
else
	$(CC) $(CFLAGS) -c $< -o $@
endif

install: $(BUILD_DIR)/$(TARGET_EXEC)
ifdef DESTDIR
	install -Dm664 $(BUILD_DIR)/$(TARGET_EXEC) ${DESTDIR}$(TARGET_EXEC)
	chmod +x $(DESTDIR)$(TARGET_EXEC)
else
	install -Dm664 $(BUILD_DIR)/$(TARGET_EXEC) /usr/bin
	chmod +x /usr/bin/$(TARGET_EXEC)
endif

clean:
	$(RM) -r $(BUILD_DIR)
