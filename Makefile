#
# This is the directory where LLVM lies in
LLVM_SRC_PATH := $$HOME/Workspace/LLVM380

LLVM_BIN_PATH := $(LLVM_SRC_PATH)/bin
LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --system-libs --libs Analysis Core ExecutionEngine InstCombine Object RuntimeDyld ScalarOpts Support native`

SRC_PATH := ./src

CXX := clang++
CXXFLAGS := -fno-rtti -O3 -g -std=c++11

SRC := $(wildcard $(SRC_PATH)/*.cc)
BIN := kal

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $(BIN)

object: $(SRC)
	$(CXX) $(CXXFLAGS) -c $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM $(BIN)
