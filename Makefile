
CC = gcc
#CFLAGS = -Wall -Werror -Wextra -pedantic -Iinclude
CFLAGS = -Iinclude -g #-fsanitize=address
LIBS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm

TARGET = sdl_program

SRCDIR = src
OBJDIR = obj

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(TARGET) $(OBJDIR)

.PHONY: all clean
