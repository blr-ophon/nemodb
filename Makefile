CC = gcc
OPT = -O0
CFLAGS = -std=gnu99 -g -Wall -Wextra -pedantic $(OPT)

INCLUDES= -I./include 

SRC_DIR := ./src
BUILD_DIR := ./build

CFILES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
EXEC := ./bin/kvsdb

${EXEC}: ${OBJECTS} ${LIBHHL} 
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} ${OBJECTS} -o $@ 

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@ 

clean:
	rm -rf ${OBJECTS} 
	rm -rf ${EXEC}

run: ${EXEC}
	$^ 

debug: ${EXEC}
	cgdb --args $^ 
