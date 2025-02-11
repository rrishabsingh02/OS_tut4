# Compiler settings for macOS
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = 
LIBS = -lm               # Link the math library (in case needed)
SOURCES = jeopardy.c questions.c players.c
OBJECTS = $(SOURCES:.c=.o)
EXE = jeopardy

.PHONY: clean help

# Build the executable
$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(EXE)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build all
all: $(EXE)

# Clean object and executable files
clean:
	rm -f $(OBJECTS) $(EXE) *~

# Clean only object files
cleanup:
	rm -f $(OBJECTS) *~

# Help menu
help:
	@echo "Valid targets:"
	@echo "  all:    generates the executable file"
	@echo "  clean:  removes .o and the executable file"
