CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Werror -pedantic -O2

all: chess

clean:
	rm -rf *.o

chess: main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o perft.o io.o
	$(CXX) main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o perft.o io.o -o chess

main: main.cc
	$(CXX) $(CXXFLAGS) main.cc

init: init.cc
	$(CXX) $(CXXFLAGS) init.cc

board: board.cc
	$(CXX) $(CXXFLAGS) board.cc

bitboard: bitboard.cc
	$(CXX) $(CXXFLAGS) bitboard.cc

move: move.cc
	$(CXX) $(CXXFLAGS) move.cc

movelist: movelist.cc
	$(CXX) $(CXXFLAGS) movelist.cc

movegen: movegen.cc
	$(CXX) $(CXXFLAGS) movegen.cc

makemove: makemove.cc
	$(CXX) $(CXXFLAGS) makemove.cc

io: io.cc
	$(CXX) $(CXXFLAGS) io.cc

perft: perft.cc
	$(CXX) $(CXXFLAGS) perft.cc
