CC=gcc
CFLAGS=-Wall -Wextra -Werror -g -Iinclude
SANITIZE=-fsanitize=address,undefined

SRC=src/chip8.c
TEST_SRC=tests/test_chip8.c tests/unity/unity.c

TARGET=chip8
TEST_TARGET=chip8_tests

all:
	$(CC) $(CFLAGS) $(SRC) src/main.c -o $(TARGET)

tests:
	$(CC) $(CFLAGS) $(SANITIZE) $(SRC) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)