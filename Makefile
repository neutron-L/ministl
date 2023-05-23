IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -DDEBUG -O2 -g -Wall -std=c++17
STL = stl
PROGRAM = test_traits test_construct test_iterator test_array test_vector
BIN = bin

all: $(PROGRAM)

test_traits: $(TEST)/test_traits.cc $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^ 

test_construct: $(TEST)/test_construct.cc $(STL)/construct.hh $(STL)/array.hh $(STL)/complex.hh $(STL)/string.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_iterator: $(TEST)/test_iterator.cc $(STL)/iterator.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_array: $(TEST)/test_array.cc $(STL)/array.hh $(TEST)/type.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_vector: $(TEST)/test_vector.cc $(STL)/vector.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	-rm $(BIN)/test_*