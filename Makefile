CXX=g++
CXXFLAGS=-std=c++0x -c -Wall

all: chess

chess: main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o undo.o perft.o
	$(CXX) main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o undo.o perft.o -o chess

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

undo: undo.cc
	$(CXX) $(CXXFLAGS) undo.cc

perft: perft.cc
	$(CXX) $(CXXFLAGS) perft.cc

clean:
	rm -rf *.o
