CC = gcc
CFLAGS = -g -Wall

# List of source files
SRCS = main.c source/utilities.c source/error_hand.c source/escape.c source/term.c source/menu.c source/book.c

# Generate a list of object files by replacing .c with .o
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# The target executable
TARGET = ./debug/a.out

# Object directory
OBJDIR = ~/code/main/object

# Source directory
SRCDIR = ~/code/main/source

# Rule to create the object directory
$(shell mkdir -p $(OBJDIR))

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
