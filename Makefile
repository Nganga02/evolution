# CC = gcc
# CFLAGS = -Wall -std=c17 -pedantic -O2 $(shell sdl2-config --cflags) -Iinclude
# LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image

# # Directories
# OBJDIR = obj
# BINDIR = bin
# SRCDIR = src
# INCDIR = include

# # Target executable
# TARGET = $(BINDIR)/evolution

# # Source files
# SOURCES = evolution.c \
# 	world-animation.c \
# 	read.c \
# 	write.c \
# 	linked-list.c \
# 	xmalloc.c \
# 	fetch_line.c

# # Object files

# # Header files (with proper path)
# HEADERS = $(INCDIR)/evolution.h \
# 	$(INCDIR)/world-animation.h \
# 	$(INCDIR)/read.h \
# 	$(INCDIR)/write.h \
# 	$(INCDIR)/linked-list.h \
# 	$(INCDIR)/xmalloc.h \
# 	$(INCDIR)/fetch_line.h

# all: $(TARGET)

# $(OBJDIR):
#     mkdir -p $(OBJDIR)

# $(BINDIR):
#     mkdir -p $(BINDIR)

# $(TARGET): $(OBJECTS) | $(BINDIR)
#     $(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# $(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
#     $(CC) $(CFLAGS) -g -c $< -o $@



# clean:
# 	rm -rf $(OBJDIR) $(BINDIR)

# .PHONY: all clean

CC = gcc
CFLAGS = -Wall -std=c17 -pedantic -O2 $(shell sdl2-config --cflags) -I$(INCDIR)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image

# Directories
OBJDIR = obj
BINDIR = bin
SRCDIR = src
INCDIR = include

# Target executable
TARGET = $(BINDIR)/evolution

# Source files
SOURCES = evolution.c \
	world-animation.c \
	read.c \
	write.c \
	linked-list.c \
	xmalloc.c \
	fetch_line.c

# Full paths for source files
SRCFILES = $(addprefix $(SRCDIR)/,$(SOURCES))

# Object files
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))

# Header files (used for dependency tracking)
HEADERS = $(wildcard $(INCDIR)/*.h)

# Default target
all: $(TARGET)

# Make directories if not exist
$(OBJDIR):
	@mkdir -p $@

$(BINDIR):
	@mkdir -p $@

# Link the executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -c $< -o $@

# Clean build
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
