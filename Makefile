IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -Og -g -Wall -std=c++11

PROGRAM = test_construct

test_construct: $(TEST)/test_construct.cc $(STL)/construct.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^