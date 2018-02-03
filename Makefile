CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = $(wildcard *.c)

all:
	$(CC) $(CFLAGS) $(SRCS) -o merge_host.exe

mem:
	drmemory.exe merge_host.exe

clean:
	del *.exe
