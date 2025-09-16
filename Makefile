CC = gcc
CFLAGS = -O2 -Wall -std=c11 `pkg-config --cflags libcurl ncurses` -Iinclude
LDLIBS = `pkg-config --libs libcurl ncurses` -lpthread
SRC = src/main.c src/network.c src/logic.c src/logger.c src/config.c src/theme.c src/ui.c
OBJ = $(SRC:.c=.o)
TARGET = pingstat

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
