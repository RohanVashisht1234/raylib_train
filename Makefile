CC = clang++
CFLAGS = -std=c++23 -I/opt/homebrew/include $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)
SRC = ./src/main.cpp ./src/libs/loaders.cpp

# Target to build the executable
game:
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS) -Wall

# Run the program
run: game
	./game

# Clean up executable
clean:
	rm -f game
