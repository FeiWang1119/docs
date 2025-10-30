#include <condition_variable>
#include <mutex>
#include <thread>

// First option, it could keep checking a flag in shared data (protected by a mutex) and
// have the second thread set the flag when it completes the task.
// This is wasteful on two counts: the thread consumes valuable processing time repeatedly checking
// the flag, and when the mutex is locked by the waiting thread, it can’t be locked by any other
// thread

// Second option is to have the waiting thread sleep for short periods between the checks using the
// std::this_thread::sleep_for() function
bool flag;
std::mutex m;

void wait_for_flag()
{
    std::unique_lock<std::mutex> lk(m);
    // In the loop, the function unlocks the mutex before the sleep, and locks it again afterward so
    // another thread gets a chance to acquire it and set the flag.
    while (!flag)
    {
        lk.unlock();
        //  This is an improvement because the thread doesn’t waste processing time while it’s
        // sleeping, but it’s hard to get the sleep period right.
        // Too short a sleep in between checks and the thread still wastes processing time checking;
        // too long a sleep and the thread will keep on sleeping even when the task it’s waiting for
        // is complete, introducing a delay.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
}

// The third and preferred option is to use the facilities from the C++ Standard Library to wait for
// the event itself. (condition variables)
// Listing 4.1 Waiting for data to process with std::condition_variable
std::mutex mut;
std::queue<data_chunk> data_queue; // a queue that’s used to pass the data between the two threads
std::condition_variable data_cond;

void data_preparation_thread()
{
    while (more_data_to_prepare())
    {
        data_chunk const data = prepare_data();
        {
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(data);
        }
        // notify the condition variable after unlocking the mutex
        // if the waiting thread wakes immediately, it doesn’t then have to block again, waiting for
        // you to unlock the mutex
        data_cond.notify_one();
    }
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut);
        // passing in the lock object and a lambda function that expresses the condition being
        // waited for
        // When the condition variable is notified by a call to notify_one() from the
        // data-preparation thread, the thread wakes from its slumber (unblocks it), reacquires the
        // lock on the mutex, and checks the condition again, returning from wait() with the mutex
        // still locked if the condition has been satisfied. If the condition hasn’t been satisfied,
        // the thread unlocks the mutex and resumes waiting.
        // This is why you need the std::unique_lock rather than the std::lock_guard—the waiting
        // thread must unlock the mutex while it’s waiting and lock it again afterward, and
        // std::lock_guard doesn’t provide that flexibility
        data_cond.wait(lk, [] {
            return !data_queue.empty();
        });
        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);
        if (is_last_chunk(data))
            break;
    }
}

// Listing 4.5 Full class definition of a thread-safe queue using condition variables
template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() { }

    threadsafe_queue(threadsafe_queue const &other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {
            return !data_queue.empty();
        });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {
            return !data_queue.empty();
        });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

int main() { }
