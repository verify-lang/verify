CC = clang
CFLAGS = -Wall -Wextra -g -Iinclude -I/usr/include/llvm-c-14 -I/usr/include/llvm-14
LDFLAGS = -lfl $(shell llvm-config --ldflags --libs core)

SRCDIR = src
INCDIR = include
OBJDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

YACC = bison
YACCFLAGS = -d -v
LEX = flex

TARGET = $(OBJDIR)/verify

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJDIR)/y.tab.o $(OBJDIR)/lex.yy.o $(OBJECTS)
	$(CC) $(OBJECTS) $(OBJDIR)/y.tab.o $(OBJDIR)/lex.yy.o -o $@ $(LDFLAGS)

$(OBJDIR)/y.tab.o: $(SRCDIR)/y.tab.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/lex.yy.o: $(SRCDIR)/lex.yy.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/y.tab.c $(SRCDIR)/y.tab.h: $(SRCDIR)/parser.y
	$(YACC) $(YACCFLAGS) -o $(SRCDIR)/y.tab.c $<

$(SRCDIR)/lex.yy.c: $(SRCDIR)/lexer.l $(SRCDIR)/y.tab.h
	$(LEX) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
	rm -f $(SRCDIR)/y.tab.c $(SRCDIR)/y.tab.h $(SRCDIR)/y.output $(SRCDIR)/lex.yy.c

.SECONDARY: $(SRCDIR)/y.tab.c $(SRCDIR)/y.tab.h $(SRCDIR)/lex.yy.c
