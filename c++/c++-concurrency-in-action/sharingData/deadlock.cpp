#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex1;
std::mutex mutex2;

// 当两个线程以不同的顺序请求相同的两把锁时，最经典的死锁情况
void threadFunctionA() {
    // 线程A先锁mutex1，再锁mutex2
    std::cout << "Thread A: Trying to lock mutex1..." << std::endl;
    std::lock_guard<std::mutex> lock1(mutex1);
    std::cout << "Thread A: Locked mutex1." << std::endl;

    // 模拟一些工作，确保线程B有机会锁住mutex2
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread A: Trying to lock mutex2..." << std::endl;
    std::lock_guard<std::mutex> lock2(mutex2); // 这里会死等，因为锁在B手里
    std::cout << "Thread A: Locked mutex2." << std::endl;

    std::cout << "Thread A: Doing work..." << std::endl;
}

void threadFunctionB() {
    // 线程B先锁mutex2，再锁mutex1
    std::cout << "Thread B: Trying to lock mutex2..." << std::endl;
    std::lock_guard<std::mutex> lock2(mutex2);
    std::cout << "Thread B: Locked mutex2." << std::endl;

    // 模拟一些工作，确保线程A有机会锁住mutex1
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread B: Trying to lock mutex1..." << std::endl;
    std::lock_guard<std::mutex> lock1(mutex1); // 这里会死等，因为锁在A手里
    std::cout << "Thread B: Locked mutex1." << std::endl;

    std::cout << "Thread B: Doing work..." << std::endl;
}

int main() {
    std::thread t1(threadFunctionA);
    std::thread t2(threadFunctionB);

    t1.join();
    t2.join();

    std::cout << "Main: Both threads completed." << std::endl;
    return 0;
}
