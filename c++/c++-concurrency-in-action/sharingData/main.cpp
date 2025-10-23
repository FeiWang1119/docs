#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <string>

// Listing 3.1 Protecting a list with a mutex
std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    // std::lock_guard guard(some_mutex); c++ 17 support class template argument deduction,
    // std::scoped_lock guard(some_mutex); C++17 also introduces an enhanced version of lock guard
    // called std::scoped_lock
    std::lock_guard<std::mutex> guard(some_mutex);

    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

// Listing 3.2 Accidentally passing out a reference to protected data
// Don’t pass pointers and references to protected data outside the scope of the lock, whether by
// returning them from a function, storing them in externally visible memory, or passing them as
// arguments to user-supplied functions.
class some_data
{
    int a;
    std::string b;

public:
    void do_something();
};

class data_wrapper
{
private:
    some_data data;
    std::mutex m;

public:
    template<typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m);
        func(data); // Pass "protected" data to user-supplied function
    }
};

some_data *unprotected;

void malicious_function(some_data &protected_data)
{
    unprotected = &protected_data;
}

data_wrapper x;

void foo()
{
    x.process_data(malicious_function); // Pass in a malicious function
    unprotected->do_something();        // Unprotected access to protected data
}

// Listing 3.3 The interface to the std::stack container adapter
template<typename T, typename Container = std::deque<T> >
class stack
{
public:
    explicit stack(const Container &);
    explicit stack(Container && = Container());
    template<class Alloc>
    explicit stack(const Alloc &);
    template<class Alloc>
    stack(const Container &, const Alloc &);
    template<class Alloc>
    stack(Container &&, const Alloc &);
    template<class Alloc>
    stack(stack &&, const Alloc &);
    bool empty() const;
    size_t size() const;
    T &top();
    T const &top() const;
    void push(T const &);
    void push(T &&);
    void pop();
    void swap(stack &&);
    template<class... Args>
    void emplace(Args &&...args);
};

void do_something(int value)
{
    std::cout << "do_something(" << value << ")" << std::endl;
}

void test0()
{
    // the use of a mutex internally to protect the stack contents doesn’t prevent race conditions
    // it is consequence of the interface
    // there might be a call to pop() from another thread that removes the last
    // element in between the call to empty() 1 and the call to top() 1.
    stack<int> s;
    if (!s.empty()) // 1
    {
        int const value = s.top(); // 2 calling top() on an empty stack is undefined behavior
        s.pop();                   // 3
        do_something(value);
    }
}

// OPTION 1: PASS IN A REFERENCE
// OPTION 2: REQUIRE A NO-THROW COPY CONSTRUCTOR OR MOVE CONSTRUCTOR
// OPTION 3: RETURN A POINTER TO THE POPPED ITEM
// OPTION 4: PROVIDE BOTH OPTION 1 AND EITHER OPTION 2 OR 3
// Listing 3.4 An outline class definition for a thread-safe stack
struct empty_stack : std::exception
{
    const char *what() const noexcept;
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack &);
    threadsafe_stack &operator=(const threadsafe_stack &) = delete;
    void push(T new_value);
    std::shared_ptr<T> pop(); // option 3: Return a shared_ptr to the value
    void pop(T &value); // option 1: Take a reference to a location in which to store the value
    bool empty() const;
};

// Listing 3.5 The implementation of the threadsafe_stack class
struct empty_stack : std::exception
{
    const char *what() const throw();
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;

public:
    threadsafe_stack() { }

    threadsafe_stack(const threadsafe_stack &other)
    {
        // Copy performed in constructor body
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack &operator=(const threadsafe_stack &) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        // Check for empty before trying to pop value
        if (data.empty())
            throw empty_stack();

        // Allocate return value before modifying stack
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

// The common advice for avoiding deadlock is to always lock the two mutexes in the same order:
// std::lock—a function that can lock two or more mutexes at once without risk of deadlock.
// Listing 3.6 Using std::lock() and std::lock_guard in a swap operation

class some_big_object;
void swap(some_big_object &lhs, some_big_object &rhs);

class X
{
private:
    some_big_object some_detail;
    std::mutex m;

public:
    X(some_big_object const &sd)
        : some_detail(sd)
    {
    }

    friend void swap(X &lhs, X &rhs)
    {
        if (&lhs == &rhs)
            return;
        std::lock(lhs.m, rhs.m); // locks the two mutexes
        // indicate to the std::lock_guard objects that the mutexes are already locked, and they
        // should adopt the ownership of the existing lock on the mutex
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        swap(lhs.some_detail, rhs.some_detail);
    }

    friend void swap_C17(X &lhs, X &rhs)
    {
        if (&lhs == &rhs)
            return;
        std::scoped_lock guard(
            lhs.m,
            rhs.m); // locks the two mutexes using the same algorithm as std::lock, c++17 provides
        swap(lhs.some_detail, rhs.some_detail);
    }
};

// Further guidelines for avoiding deadlock
// 1. don’t wait for another thread if there’s a chance it’s waiting for you
// 2. AVOID NESTED LOCKS: don’t acquire a lock if you already hold one
// 3. AVOID CALLING USER-SUPPLIED CODE WHILE HOLDING A LOCK
// 4. ACQUIRE LOCKS IN A FIXED ORDER
//
// Listing 3.8 A simple hierarchical mutex
class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    // This value is accessible to all mutex instances, but has a different value on each thread
    // This allows the code to check the behavior of each thread separately,
    // and the code for each mutex can check whether or not the current thread is allowed to lock
    // that mutex.
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation()
    {
        if (this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit hierarchical_mutex(unsigned long value)
        : hierarchy_value(value)
        , previous_hierarchy_value(0)
    {
    }

    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        // In order to avoid the hierarchy getting confused due to out-of-order unlocking,
        // you throw at if the mutex being unlocked is not the most recently locked one.
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated");
        // it’s important to save the previous value of the hierarchy value for the current
        // thread so you can restore it in unlock(); otherwise you’d never be able to
        // lock a mutex with a higher hierarchy value again, even if the thread didn’t hold any locks.
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    // if the lock on the mutex is held by another thread,
    // it returns false rather than waiting until the calling thread can acquire the lock on the mutex.
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};

// It’s initialized to the maximum value, so initially any mutex can be locked.
unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

// Listing 3.7 Using a lock hierarchy to prevent deadlock
// When code tries to lock a mutex, it isn’t permitted to lock that mutex if it already holds a lock
// from a lower layer. Deadlocks between hierarchical mutexes are impossible, because the mutexes
// themselves enforce the lock ordering.
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);
int do_low_level_stuff(); // Assuming do_low_level_stuff doesn’t lock any mutexes,

int low_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_stuff(int some_param);

void high_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}

void thread_a()
{
    high_level_func();
}

void do_other_stuff();

void other_stuff()
{
    high_level_func();
    do_other_stuff();
}

void thread_b()
{
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}

int main() { }
