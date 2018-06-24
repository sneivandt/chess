CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Werror -pedantic -O3
NAME=ace
OBJS=$(patsubst %.cc, %.o, $(wildcard *.cc))

.PHONY: all build clean debug

debug: CXXFLAGS+=-g
debug: all

all: build

build: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf *.o $(NAME)
