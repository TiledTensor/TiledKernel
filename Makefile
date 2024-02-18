CC := g++
EXAMPLE ?= shared_graph
EXAMPLE_SRCS := $(wildcard examples/*.cc)
EXAMPLES := $(patsubst examples/%.cc, %, $(EXAMPLE_SRCS))

LD_FLAGS := -Lbuild/ -ltiledkernel -Wl,-rpath,build/
INC_FLAGS := -Iinclude -I3rd-party/fmt/include -I3rd-party/fmtlog
MACRO_FLAGS := -DFMT_HEADER_ONLY -DFMTLOG_HEADER_ONLY

CMAKE_OPTIONS :=

CUDA ?= ON
DEBUG ?= OFF

ifeq ($(DEBUG), ON)
	CMAKE_OPTIONS += -DDEBUG=ON
endif

BUILD := build

.PHONY: build example 

build:
	@mkdir -p build
	cd build && cmake $(CMAKE_OPTIONS) .. && make

example: build
	@$(CC) -std=c++17 examples/$(EXAMPLE).cc $(INC_FLAGS) $(LD_FLAGS) $(MACRO_FLAGS) -o build/$(EXAMPLE)
	@./build/$(EXAMPLE)

test: build
	@cd build && make test

clean:
	@rm -rf build