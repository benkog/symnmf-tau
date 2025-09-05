CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
OBJECTS = symnmf.o utils.o
TARGET = symnmf

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
