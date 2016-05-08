LLVM_SRC_PATH := $$HOME/Workspace/LLVM380
LLVM_BIN_PATH := $(LLVM_SRC_PATH)/bin
LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --system-libs --libs core mcjit native`

INC_PATH := ./include
SRC_PATH := ./src

CXX := clang++
CXXFLAGS := -fno-rtti -O3 -g -std=c++11 -I $(INC_PATH)

SRC := $(wildcard $(SRC_PATH)/*.cc)
BIN := kal

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM $(BIN)
