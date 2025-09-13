CC = gcc
CFLAGS = -Wall -g -std=c99 -D_DEFAULT_SOURCE
LIBS = -lcurl -ljson-c -lncursesw -lpthread -lconfig
INCLUDES = -I./include

SRC = src/main.c src/network.c src/theme.c src/ui.c src/logic.c src/config.c
OBJ = $(SRC:.c=.o)
TARGET = ping-status

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	mkdir -p /usr/local/share/ping-status/css
	mkdir -p /usr/local/share/ping-status/js
	cp css/* /usr/local/share/ping-status/css/
	cp js/* /usr/local/share/ping-status/js/
	cp -r themes /usr/local/share/ping-status/

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: install clean