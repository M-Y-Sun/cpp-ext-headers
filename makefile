CC = clang++
CXXFLAGS = -I./$(INCL) -g -Wall -Wextra -Wpedantic -std=c++20

MAIN ?=  src/main.cc
SRC_DIR = src/impl
SRC_FILES := $$(find $(SRC_DIR) -name '*.cc' | xargs)
INCL := src/include/
BIN ?= bin/main.elf

default: $(MAIN)
	if [ ! -d bin/ ]; then mkdir bin/; fi
	if [ ! -d dbg/ ]; then mkdir dbg/; fi

	$(CC) $(MAIN) $(SRC_FILES) -o $(BIN) $(CXXFLAGS) 

	if [ -d dbg/main.elf.dSYM/ ]; then rm -r dbg/main.elf.dSYM/; fi
	if [ -d bin/main.elf.dSYM/ ]; then mv bin/main.elf.dSYM/ dbg//; fi

verbose: $(MAIN)
	if [ ! -d bin/ ]; then mkdir bin/; fi
	if [ ! -d dbg/ ]; then mkdir dbg/; fi

	$(CC) $(MAIN) $(SRC_DIR)/*.cc -o $(BIN) $(CXXFLAGS) -v

	if [ -d dbg/main.elf.dSYM/ ]; then rm -r dbg/main.elf.dSYM/; fi
	if [ -d bin/main.elf.dSYM/ ]; then mv bin/main.elf.dSYM/ dbg//; fi

clean: $(BIN) $(BIN).dSYM/
	rm $(BIN)
	rm -r $(BIN).dSYM/
	
