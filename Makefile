CC = g++
CFLAGS = -g -Wall -lmariadbcpp
STD = -std=c++11
NAME = IPParser

.PHONY: all clean
SOURCES = main.cpp
OBJECTS = main.o


all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o ./bin/$(NAME) ./src/$(OBJECTS) $(STD)

main.o: ./src/$(SOURCES)
	$(CC) $(CFLAGS) -c ./src/$(SOURCES) -o ./src/$@ $(STD)
  
clean:
	rm -f ./src/$(OBJECTS) ./bin/$(NAME)