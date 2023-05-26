# ministl
## Introduction
STL库中的常见容器和算法的实现，主要包括四部分：
- 容器：包括序列式容器和关联式容器
- 迭代器：
- 算法：
- 测试：在tests/目录下，文件名称统一为test_xxxx.cc 

## Container
容器的实现主要在stl/目录下，以容器的名称命名文件，如vector的实现在vector.hh中，list的实现在list.hh中。预计实现四种类型的容器
### Sequence
- array
- vector            
- forward list      (TODO)
- list              (TODO)
- deque             (TODO)  

### Associative 
- unordered_set
- unordered_multiset
- unordered_map
- unordered_multimap  

### Unordered associative
- set
- multiset
- map
- multimap  

### Adaptors
- stack
- queue
- priority_queue

## Iterators


## Algorithm

## test cases
几乎对于每个stl/目录下的文件中的算法或容器的实现，都有一个对应的测试程序位于tests/目录下，格式一般为test_xxxx.cc。比如，对于vector的实现，测试程序为test_vector.cc；对于iterator的实现，测试程序为test_iterator.cc。以vector测试为例，运行测试程序的方法为：
在主目录（ministl）下，运行make指令：
```c++
$ make test_vector
```
目标文件和可执行文件被放置在bin/目录下，运行可执行文件：
```
$ ./bin/test_vector
```
可在终端查看测试的情况。
