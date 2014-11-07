CXX=g++
CXXFLAGS=-std=c++0x -Wall -Wextra -Werror -pedantic -O3

all: chess

chess: main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o perft.o io.o search.o utils.o pvtable.o searchinfo.o evaluate.o
	$(CXX) main.o init.o board.o bitboard.o move.o movelist.o movegen.o makemove.o perft.o io.o search.o utils.o pvtable.o searchinfo.o evaluate.o -o chess

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

perft: perft.cc
	$(CXX) $(CXXFLAGS) perft.cc

io: io.cc
	$(CXX) $(CXXFLAGS) io.cc

search: search.cc
	$(CXX) $(CXXFLAGS) search.cc

utils: utils.cc
	$(CXX) $(CXXFLAGS) utils.cc

pvtable: pvtable.cc
	$(CXX) $(CXXFLAGS) pvtable.cc

searchinfo: searchinfo.cc
	$(CXX) $(CXXFLAGS) searchinfo.cc

evaluate: evaluate.cc
	$(CXX) $(CXXFLAGS) evaluate.cc

clean:
	rm -rf *.o
