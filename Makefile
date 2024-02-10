CC := g++
EXAMPLE ?= edge
EXAMPLE_SRCS := $(wildcard examples/*.cc)
EXAMPLES := $(patsubst examples/%.cc, %, $(EXAMPLE_SRCS))

LD_FLAGS := -Lbuild/ -ltiledkernel -Wl,-rpath=build/
INC_FLAGS := -Iinclude
# MACRO_FLAGS := -DFMTLOG_HEADER_ONLY -DFMT_HEADER_ONLY

BUILD := build

.PHONY: build example 

build:
	@mkdir -p build
	@cd build && cmake.. && make

example:
	@$(CC) -std=c++17 examples/$(EXAMPLE).cc $(INC_FLAGS) $(LD_FLAGS) $(MACRO_FLAGS) -o build/$(EXAMPLE) 
	@./build/$(EXAMPLE)