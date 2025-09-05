CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

TARGET = symnmf
SRC = symnmf.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
