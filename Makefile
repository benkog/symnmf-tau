CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LFLAGS = -lm
OBJECTS = symnmf.o utils.o
TARGET = symnmf

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
