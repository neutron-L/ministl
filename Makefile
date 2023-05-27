IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -DDEBUG -O0 -g -Wall -std=c++17
STL = stl
PROGRAM = test_traits test_construct test_iterator test_array test_vector test_numeric
BIN = bin

all: $(PROGRAM)

test_traits: $(TEST)/test_traits.cc $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^ 

test_construct: $(TEST)/test_construct.cc $(STL)/complex.hh $(STL)/string.hh $(STL)/array.hh $(STL)/construct.hh $(STL)/type_traits.hh 
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_iterator: $(TEST)/test_iterator.cc $(STL)/iterator.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_array: $(TEST)/test_array.cc $(STL)/array.hh $(STL)/string.hh $(STL)/complex.hh $(STL)/uninitialized.hh $(STL)/iterator.hh $(STL)/type_traits.hh $(STL)/construct.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_vector: $(TEST)/test_vector.cc $(STL)/vector.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(STL)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_numeric: $(TEST)/test_numeric.cc $(STL)/numeric.hh $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	-rm $(BIN)/test_*