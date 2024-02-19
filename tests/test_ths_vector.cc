#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "ths_vector.hh"

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex              mtx;  // 用于保护vector的互斥锁
std::condition_variable cv;  // 用于唤醒等待的读者线程的条件变量
std::vector<int>        data;             // 要读取和写入的vector
bool                    writing = false;  // 指示是否有写者正在写入

void writer()
{
    for (int i = 0; i < 10; ++i) {
        data.push_back(i);
        std::cout << "Writer added: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void reader()
{
    std::vector<int> first, second;

    // 读取数据
    auto iter = data.begin();
    auto temp = iter;
    while (temp != data.end()) {
        first.push_back(*temp++);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // 读取数据
    temp = iter;
    while (temp != data.end()) {
        second.push_back(*temp++);
    }

    // 两次读取到的数据需要相同
    assert(first == second);
}

int main()
{
    std::thread writer_thread(writer);

    // 创建多个读者线程
    std::vector<std::thread> reader_threads;
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        reader_threads.emplace_back(reader);
    }

    // 等待所有线程完成
    writer_thread.join();
    for (auto& thread : reader_threads) {
        thread.join();
    }

    return 0;
}
