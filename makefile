CC = clang++
CXXFLAGS = -I./$(INCL) -g -Wall -Wextra -Wpedantic -std=c++20
OFLAG ?= O1

FILE = main
MAIN ?=  src/$(FILE).cc
BIN = $(FILE)

IMPL_DIR = src/impl
IMPL_FILES := $$(find $(IMPL_DIR) -name '*.cc' | xargs)
INCL := src/include/

MKDIRS = /bin/bash -c 'if [ ! -d bin/ ]; then mkdir bin/; fi; if [ ! -d dbg/ ]; then mkdir dbg/; fi'
REPLACE_FILES = /bin/bash -c 'if [ -d dbg/$(BIN).dSYM/ ]; then rm -r dbg/$(BIN).dSYM/; fi; if [ -d bin/$(BIN).dSYM/ ]; then mv bin/$(BIN).dSYM/ dbg//; fi'

CXXCPFX = $(CC) $(MAIN) $(IMPL_FILES) -o bin/$(BIN) $(CXXFLAGS)  

default: $(MAIN)
	$(MKDIRS)
	$(CXXCPFX) -$(OFLAG)
	$(REPLACE_FILES)

fast:
	$(MKDIRS)
	$(CXXCPFX) -Ofast
	$(REPLACE_FILES)

debug:
	$(MKDIRS)
	$(CXXCPFX) -Og
	$(REPLACE_FILES)

size:
	$(MKDIRS)
	$(CXXCPFX) -Os
	$(REPLACE_FILES)

verbose: $(MAIN)
	$(MKDIRS)
	$(CXXCPFX) -$(OFLAG) -v
	$(REPLACE_FILES)

clean: bin/$(BIN) bin/$(BIN).dSYM/
	rm bin/$(BIN)
	rm -r bin/$(BIN).dSYM/
	
