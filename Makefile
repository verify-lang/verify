CC = clang
CFLAGS = -Wall -Wextra -std=c99
TARGET = main
SOURCES = main.c

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean
