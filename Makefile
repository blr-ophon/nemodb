CC = gcc
OPT = -O2
CFLAGS = -std=gnu99 -g -Wall -Wextra -pedantic $(OPT)

INCLUDES= -I./include 

CFILES_DIR := ./src
BUILD_DIR := ./build

CFILES := $(wildcard ./src/*.c)
OBJECTS := $(CFILES:$(CFILES_DIR)/%.c=$(BUILD_DIR)/%.o)
EXEC := ./bin/kvsdb

${EXEC}: ${OBJECTS} ${LIBHHL} 
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} ${OBJECTS} -o $@ 

${BUILD_DIR}/%.o: ${CFILES_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@ 

clean:
	rm -rf ${OBJECTS} 
	rm -rf ${EXEC}

testrun: ${EXEC}
	$^ 

debug: ${EXEC}
	cgdb --args $^ 
