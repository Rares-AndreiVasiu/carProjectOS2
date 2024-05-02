# -*- Makefile -*-

# variables section
SRCDIR = src

BINDIR = bin

CC = gcc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lm

# targets section

all: $(BINDIR)/car

$(BINDIR)/car: $(BINDIR)/main.o $(BINDIR)/menu.o $(BINDIR)/utility.o | $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BINDIR)/main.o: $(SRCDIR)/main.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/menu.o: $(SRCDIR)/menu.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/utility.o: $(SRCDIR)/utility.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)
