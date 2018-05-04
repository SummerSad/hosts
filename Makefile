CC = gcc
CFLAGS = -Wall -Wextra
SRCS = $(wildcard src/*.c)

all:
	$(CC) $(CFLAGS) $(SRCS) -o merge_host.exe

mem:
	drmemory.exe merge_host.exe

clean:
	del *.exe
