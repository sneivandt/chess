CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Werror -pedantic -O3
NAME=ace
OBJS=$(patsubst %.cc, %.o, $(wildcard *.cc))

all: $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

%: %.cc
	$(CXX) %.cc

clean:
	rm -rf *.o $(NAME)
