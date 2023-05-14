IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -DDEBUG -O0 -g -Wall -std=c++17
STL = stl
PROGRAM = test_traits test_construct test_iterator test_array
BIN = bin

all: $(PROGRAM)

test_traits: $(TEST)/test_traits.cc $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^ 

test_construct: $(TEST)/test_construct.cc $(STL)/construct.hh $(STL)/array.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_iterator: $(TEST)/test_iterator.cc $(STL)/iterator.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_array: $(TEST)/test_array.cc $(STL)/array.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	-rm $(BIN)/test_*