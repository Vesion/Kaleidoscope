CXX=clang++
INC_DIR=./inc/
SRC_DIR=./
CXXFLAGS=-g -std=c++11 --pedantic -I$(INC_DIR)
BIN=kal

SRC=$(wildcard $(SRC_DIR)/*.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f $(SRC_DIR)*.o
	rm -f $(BIN)
