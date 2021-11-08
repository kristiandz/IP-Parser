CC = g++
CFLAGS = -g -Wall
NAME = IPParser

.PHONY: all clean
SOURCES = main.cpp
OBJECTS = main.o


all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) -std=c++11

main.o:
	$(CC) $(CFLAGS) -c $(SOURCES) -std=c++11
  
clean:
	rm -f $(OBJECTS) $(NAME)