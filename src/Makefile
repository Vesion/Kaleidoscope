UNAME := $(shell uname -s)

LLVM_SRC_PATH := $$HOME/Workspace/LLVM380
LLVM_BUILD_PATH := $(LLVM_SRC_PATH)/bin
LLVM_BIN_PATH := $(LLVM_SRC_PATH)/bin

CXX := clang++
CXXFLAGS := -fno-rtti -O0 -g -std=c++11
PLUGIN_CXXFLAGS := -fpic

LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`

LLVM_LDFLAGS_NOLIBS := `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared -Wl,-undefined,dynamic_lookup

CLANG_INCLUDES := \
	-I$(LLVM_SRC_PATH)/tools/clang/include \
	-I$(LLVM_BUILD_PATH)/tools/clang/include

ifneq ($(UNAME),Darwin)
	CLANG_LIBS += \
	-Wl,--start-group
endif

CLANG_LIBS += \
	-lclangAST \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangEdit \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangEdit \
	-lclangASTMatchers \
	-lclangRewrite \
	-lclangRewriteFrontend \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangSerialization \
	-lclangToolingCore \
	-lclangTooling

ifneq ($(UNAME),Darwin)
	CLANG_LIBS += \
		-Wl,--end-group
endif

SRC_LLVM_DIR := src_llvm
SRC_CLANG_DIR := src_clang
BUILDDIR := build


INC_DIR := ../inc
SRC_DIR := ./
BIN := kal
SRC := $(wildcard $(SRC_DIR)/*.cc)

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $^ $(LLVM_LDFLAGS) -o $(BIN)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM $(BIN)
