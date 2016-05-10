LLVM_SRC_PATH := $$HOME/Workspace/LLVM380
LLVM_BIN_PATH := $(LLVM_SRC_PATH)/bin
LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --system-libs --libs core mcjit native`

INC_PATH := ./include
SRC_PATH := ./src

CXX := clang++
CXXFLAGS := -fno-rtti -O3 -g -std=c++11 -I $(INC_PATH)

SRC := $(wildcard $(SRC_PATH)/*.cc)
JIT := $(SRC_PATH)/kaljit.cc
DEBUG := $(SRC_PATH)/kaldebug.cc
BIN := kal

jit: $(JIT)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $(BIN)

debug: $(DEBUG)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM $(BIN)
