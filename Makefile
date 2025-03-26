BINARY = objgc
SRCDIRS = . ./src
INCDIRS = . ./include

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wconversion

INCLUDES = $(foreach DIR,$(INCDIRS),-I$(DIR))
DEPFLAGS = -MP -MD
DFLAGS = -DDEBUG -g

CFLAGS += $(INCLUDES) $(DEPFLAGS)

CFILES = $(foreach DIR,$(SRCDIRS),$(wildcard $(DIR)/*.c))
OBJFILES = $(patsubst %.c, %.o, $(CFILES)) 
DEPFILES = $(patsubst %.c, %.d, $(CFILES)) 

.PHONY: all debug leak-check run clean help

all: $(BINARY)

debug: CFLAGS += $(DFLAGS)
debug: $(BINARY)

leak-check: debug
	@valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(BINARY)

$(BINARY): $(OBJFILES)
	@$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(BINARY)
	@./$(BINARY)

clean:
	@rm -rf $(BINARY) $(OBJFILES) $(DEPFILES)

help:
	@echo "Available targets:"
	@echo "  all         - Build binary (Default target)"
	@echo "  debug       - Build binary with debugging symbols"
	@echo "  leak-check  - Build binary with debugging symbols and run with valgrind"
	@echo "  run         - Build binary and execute"
	@echo "  clean       - Clean up generated files (binary, object files, dependencies)"
	@echo "  help        - Show help message"

-include $(DEPFILES)
