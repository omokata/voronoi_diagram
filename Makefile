CC = gcc
CFLAGS = -Wall -Wextra

TARGET = voronoi
SRCS = main.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)
