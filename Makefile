CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = adventureGame
SOURCES = main.c draw_map.c map_generation.c movement_system.c pathfinding.c update.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
