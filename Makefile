IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -Og -g -Wall -std=c++11
STL = stl
PROGRAM = test_construct test_iterator
BIN = bin

all: $(PROGRAM)
	

test_construct: $(TEST)/test_construct.cc $(STL)/construct.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_iterator: $(TEST)/test_iterator.cc $(STL)/iterator.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	-rm $(BIN)/test_*