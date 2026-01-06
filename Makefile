CC=gcc
CFLAGS=-Wall -Wextra -Werror -g -Iinclude
SANITIZE=-fsanitize=address,undefined

SRC=src/chip8.c
HEADER_SRC=src/chip8.h
TEST=test/test_chip8.c
TEST_SRC= $(TEST) test/unity/unity.c
EXECUTABLE=main
TARGET=chip8
TEST_TARGET=chip8_tests

all:
	$(CC) $(CFLAGS) $(SRC) src/main.c -o $(TARGET)

tests:
	$(CC) $(CFLAGS) -Itest/unity $(SANITIZE) $(SRC) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

testing:
	gdb ./$(TEST_TARGET)

git_code:
	git add $(SRC) $(HEADER_SRC) 

git_test:
	git add $(TEST)


compile:
	gcc ./src/$(EXECUTABLE).c -o ./src/$(EXECUTABLE) `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf

run:
	SDL_VIDEODRIVER=x11 ./src/$(EXECUTABLE)

clean:
	rm -f $(TARGET) $(TEST_TARGET)