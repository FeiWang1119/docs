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
        // lock a mutex with a higher hierarchy value again, even if the thread didn’t hold any
        // locks.
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    // if the lock on the mutex is held by another thread,
    // it returns false rather than waiting until the calling thread can acquire the lock on the
    // mutex.
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

// Listing 3.9 Using std::lock() and std::unique_lock in a swap operation
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
        // Pass std::defer_lock as the second argument to indicate that the mutex should remain
        // unlocked on construction
        // The lock can then be acquired later by calling lock() on the std::unique_lock object (not
        // the mutex)
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
        std::lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
};

// Transferring mutex ownership between scopes
// the get_lock() function locks the mutex and then prepares the data before returning the lock to
// the caller:
std::unique_lock<std::mutex> get_lock()
{
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    // this transfer is automatic, without a call to std:move();
    // the compiler takes care of calling the move constructor
    return lk;
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());
    // rely on the data being correctly prepared without another thread altering the data in the
    // meantime
    do_something();
}

// You don’t need the mutex locked across the call to process(), so you manually unlock it before
// the call and then lock it again afterward
void get_and_process_data()
{
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process = get_next_data_chunk();
    my_lock.unlock(); // don’t need mutex locked across the call to process()
    result_type result = process(data_to_process);
    my_lock.lock(); // Relock mutex to write result
    write_result(data_to_process, result);
}

// Listing 3.10 Locking one mutex at a time in a comparison operator
class Y
{
private:
    int some_detail;
    mutable std::mutex m;

    int get_detail() const
    {
        std::lock_guard<std::mutex> lock_a(m);
        return some_detail;
    }

public:
    Y(int sd)
        : some_detail(sd)
    {
    }

    friend bool operator==(Y const &lhs, Y const &rhs)
    {
        if (&lhs == &rhs)
            return true;
        int const lhs_value = lhs.get_detail();
        int const rhs_value = rhs.get_detail();
        return lhs_value == rhs_value;
    }
};

// Lazy initialization such as this is common in single-threaded code—each
// operation that requires the resource first checks to see if it has been initialized and then
// initializes it before use if not:
std::shared_ptr<some_resource> resource_ptr;

void foo()
{
    if (!resource_ptr)
    {
        resource_ptr.reset(new some_resource);
    }
    resource_ptr->do_something();
}

// Listing 3.11 Thread-safe lazy initialization using a mutex
std::mutex resource_mutex;

void foo1()
{
    std::unique_lock<std::mutex> lk(resource_mutex); // All threads are serialized here
    if (!resource_ptr)
    {
        resource_ptr.reset(new some_resource); // Only the initialization needs protection
    }
    lk.unlock();
    resource_ptr->do_something();
}

// infamous double-checked locking pattern:
// this pattern is infamous for a reason: it has the potential for nasty race
// conditions, because the read outside the lock, isn’t synchronized with the write
// done by another thread inside the lock. This creates a race condition that covers
// not only the pointer itself but also the object pointed to; even if a thread sees the
// pointer written by another thread, it might not see the newly created instance of
// some_resource, resulting in the call to do_something() operating on incorrect values.
// This is an example of the type of race condition defined as a data race by the C++
// Standard and specified as undefined behavior. It’s therefore quite definitely something
// to avoid.
void undefined_behaviour_with_double_checked_locking()
{
    // the pointer is first read without acquiring the lock , and the lock is acquired only if the
    // pointer is NULL.
    if (!resource_ptr)
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        // The pointer is then checked again once the lock has been acquired (hence the
        // double-checked part) in case another thread has done the initialization between the first
        // check and this thread acquiring the lock
        if (!resource_ptr)
        {
            resource_ptr.reset(new some_resource);
        }
    }
    resource_ptr->do_something();
}

std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource()
{
    resource_ptr.reset(new some_resource);
}

void foo()
{
    std::call_once(resource_flag, init_resource); // Initialization is called exactly once.
    resource_ptr->do_something();
}

// Listing 3.12 Thread-safe lazy initialization of a class member using std::call_once
class X
{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection() { connection = connection_manager.open(connection_details); }

public:
    X(connection_info const &connection_details_)
        : connection_details(connection_details_)
    {
    }

    // the initialization is done either by the first call to send_data(), or by the first call to
    // receive_data()
    void send_data(data_packet const &data)
    {
        std::call_once(connection_init_flag, &X::open_connection, this);
        connection.send_data(data);
    }

    data_packet receive_data()
    {
        std::call_once(connection_init_flag, &X::open_connection, this);
        return connection.receive_data();
    }
};

// Listing 3.13 Protecting a data structure with std::shared_mutex
class dns_entry;

class dns_cache
{
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mutex;

public:
    // find_entry() uses an instance of std::shared_lock<> to protect it for
    // shared, read-only access; multiple threads can therefore call find_entry() simultaneously
    // without problems.
    dns_entry find_entry(std::string const &domain) const
    {
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string, dns_entry>::const_iterator const it = entries.find(domain);
        return (it == entries.end()) ? dns_entry() : it->second;
    }

    // update_or_add_entry() uses an instance of std::lock_guard<> to provide exclusive access while
    // the table is updated; not only are other threads prevented from doing updates in a call to
    // update_ or_add_entry(), but threads that call find_entry() are blocked too.
    void update_or_add_entry(std::string const &domain, dns_entry const &dns_details)
    {
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
};

int main() { }
