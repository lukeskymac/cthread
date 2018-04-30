#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: basic_test

basic_test: basic_test.c $(LIB_DIR)/libcthread.a
	$(CC) -o basic_test basic_test.c -L$(LIB_DIR) -lcthread -Wall
	
clean:
	rm -rf basic_test *.o *~




