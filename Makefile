IDIR = stl
TEST = tests
BIN = bin

CXX = g++
CFLAGS = -I$(IDIR) -DDEBUG -O0 -g -Wall -std=c++17
STL = stl
PROGRAM = test_traits test_construct test_iterator test_array test_vector test_numeric test_list
BIN = bin

all: $(PROGRAM)

test_traits: $(TEST)/test_traits.cc $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^ 

test_construct: $(TEST)/test_construct.cc $(STL)/complex.hh $(STL)/string.hh $(STL)/array.hh $(STL)/construct.hh $(STL)/type_traits.hh $(STL)/algobase.hh 
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_iterator: $(TEST)/test_iterator.cc $(STL)/iterator.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_array: $(TEST)/test_array.cc $(STL)/array.hh $(STL)/string.hh $(STL)/complex.hh $(STL)/uninitialized.hh $(STL)/iterator.hh $(STL)/type_traits.hh $(STL)/construct.hh 
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_vector: $(TEST)/test_vector.cc $(STL)/vector.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

	
test_list: $(TEST)/test_list.cc $(STL)/list.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^
	
test_deque: $(TEST)/test_deque.cc $(STL)/deque.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_stack: $(TEST)/test_stack.cc $(STL)/stack.hh  $(STL)/deque.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_queue: $(TEST)/test_queue.cc $(STL)/queue.hh  $(STL)/deque.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_rbtree: $(TEST)/test_rbtree.cc $(STL)/rbtree.hh $(STL)/vector.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_set: $(TEST)/test_set.cc $(STL)/set.hh $(STL)/rbtree.hh $(STL)/deque.hh $(STL)/vector.hh $(STL)/alloc.hh $(STL)/iterator.hh $(STL)/uninitialized.hh $(TEST)/type.hh $(TEST)/utils.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^


test_numeric: $(TEST)/test_numeric.cc $(STL)/numeric.hh $(STL)/type_traits.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

test_algobase: $(TEST)/test_algobase.cc $(STL)/list.hh $(STL)/vector.hh $(STL)/deque.hh $(STL)/algobase.hh
	$(CXX) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	-rm $(BIN)/test_*