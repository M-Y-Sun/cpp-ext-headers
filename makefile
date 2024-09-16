CC = clang++
CFLAGS = -I./$(INCL) -g -Wall -Wextra -Wpedantic -std=c++20

MAIN ?=  src/main.cc
SRC_DIR = src/implm
SRC_FILES := $$(find $(SRC_DIR) -name '*.cc' | xargs)
INCL := src/include/
BIN ?= bin/main.elf

default: $(MAIN)
	if [ ! -d bin/ ]; then mkdir bin/; fi
	if [ ! -d dbg/ ]; then mkdir dbg/; fi

	$(CC) $(MAIN) $(SRC_FILES) -o $(BIN) $(CFLAGS) 

	if [ -d dbg/main.elf.dSYM/ ]; then rm -r dbg/main.elf.dSYM/; fi
	if [ -d bin/main.elf.dSYM/ ]; then mv bin/main.elf.dSYM/ dbg//; fi

verbose: $(MAIN)
	if [ ! -d bin/ ]; then mkdir bin/; fi
	if [ ! -d dbg/ ]; then mkdir dbg/; fi

	$(CC) $(MAIN) $(SRC_DIR)/*.cc -o $(BIN) $(CFLAGS) -v

	if [ -d dbg/main.elf.dSYM/ ]; then rm -r dbg/main.elf.dSYM/; fi
	if [ -d bin/main.elf.dSYM/ ]; then mv bin/main.elf.dSYM/ dbg//; fi

clean: $(BIN) $(BIN).dSYM/
	rm $(BIN)
	rm -r $(BIN).dSYM/
	
