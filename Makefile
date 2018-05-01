CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
CFLAGS=-c

all:  libcthread

libcthread: cthread.o
	mv *.o $(BIN_DIR)
	ar rcs $(LIB_DIR)/libcthread.a $(BIN_DIR)/cthread.o $(BIN_DIR)/support.o

cthread.o:
	$(CC) $(CFLAGS) -m32 -I$(INC_DIR) $(SRC_DIR)/cthread.c -Wall

clean:
	mv $(BIN_DIR)/support.o .
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	mv support.o $(BIN_DIR)