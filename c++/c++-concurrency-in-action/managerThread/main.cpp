#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <numeric>

using namespace std;

void do_some_work()
{
    cout << "do_some_work" << endl;
}

void do_something_in_current_thread()
{
    cout << "do_something_in_current_thread" << endl;
}

class background_task
{
public:
    void operator()() const { cout << "background_task" << endl; }
};

// access to dangling reference (undefined behavior)
// 1. One common way to handle this scenario is to make the thread function self-contained
// and copy the data into the thread rather than sharing the data.
// 2. Alternatively, you can ensure that the thread has completed execution before the
// function exits by joining with the thread.
struct func
{
    int &i;

    func(int &i_)
        : i(i_)
    {
    }

    void operator()() const
    {
        for (unsigned int j = 0; j < 100000; ++j)
        {
            cout << j << endl;
            cout << i << endl; // potential access to dangling reference
        }
    }
};

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    thread my_thread(my_func);
    my_thread.detach(); // do not wait for thread to finish, new thread might still be running
}

// Waiting in exceptional circumstances
// it’s important to ensure that the thread completes before the function exits—whether because it
// has a reference to other local variables or for any other reason—then. it’s important to ensure
// this is the case for all possible exit paths, whether normal or exceptional, and it’s desirable
// to provide a simple, concise mechanism for doing so.

void f()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        do_something_in_current_thread(); // might throw an exception
    }
    catch (...)
    {
        t.join();
        throw;
    }
    t.join();
}

// One way of doing this is to use the standard Resource Acquisition Is Initialization
// (RAII) idiom and provide a class that does the join() in its destructor, as in the following
// listing.

class thread_guard
{
    std::thread &t;

public:
    explicit thread_guard(std::thread &t_)
        : t(t_)
    {
    }

    ~thread_guard()
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;
};

void f1()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread(); // might throw an exception
}

// Passing arguments to a thread function
void f2(int i, std::string const &s) { }

void oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(f2, 3, buffer);
    t.detach();
}

// it’s the pointer to the local variable buffer that’s passed through to the
// new thread and there’s a significant chance that the oops function will exit before
// the buffer has been converted to a std::string on the new thread, thus leading to
// undefined behavior. The solution is to cast to std::string before passing the buffer
// to the std::thread constructor

void not_oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(f2, 3, std::string(buffer)); // Using std::string avoids dangling pointer
    t.detach();
}

using widget_id = int;

class widget_data
{
};

void update_data_for_widget(widget_id w, widget_data &data) { }

void display_status() { }

void process_widget_data(widget_data &data) { }

void oops_again(widget_id w)
{
    widget_data data;
    // the internal code passes copied arguments as rvalues in order to work with move-only
    // types, and will thus try to call update_data_for_widget with an rvalue. This will fail to
    // compile because you can't pass an rvalue to a function that expects a non-const reference.
    // std::thread t(update_data_for_widget, w, data);
    std::thread t(update_data_for_widget, w, std::ref(data));
    display_status();
    t.join();
    process_widget_data(data);
}

class X
{
public:
    void do_lengthy_work() { }
};

// pass member function to thread
void test0()
{
    X my_x;
    std::thread t(&X::do_lengthy_work, &my_x);
}

class big_object
{
public:
    void prepare_data(int) { }
};

void process_big_object(std::unique_ptr<big_object>) { }

// pass unique_ptr to thread
void test1()
{
    std::unique_ptr<big_object> ptr(new big_object);
    ptr->prepare_data(42);
    std::thread tt(process_big_object, std::move(ptr));
}

void some_function() { }

void some_other_function() { }

// Transferring ownership of a thread
void test2()
{
    std::thread t1(some_function);
    std::thread t2 = std::move(t1); // t1 -> t2
    t1 = std::thread(some_other_function);
    std::thread t3;
    t3 = std::move(t2); // t2 -> t3
    t1 = std::move(t3); // t3 (some_function) -> t1 (some_other_function) This assignment will
                        // terminate the program!
    // some_other_function thread has no thread object to manage.
}

// Returning a std::thread from a function
std::thread f2()
{
    void some_function();
    return std::thread(some_function);
}

void some_other_function1(int) { }

std::thread g()
{
    std::thread t(some_other_function1, 42);
    return t;
}

// Ownership should be transferred into a function
void f(std::thread t) { }

void g0()
{
    void some_function();
    f(std::thread(some_function));
    std::thread t(some_function);
    // f(t);
    f(std::move(t));
}

class scoped_thread
{
    std::thread t;

public:
    explicit scoped_thread(std::thread t_)
        : t(std::move(t_))
    {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }

    ~scoped_thread() { t.join(); }

    scoped_thread(scoped_thread const &) = delete;
    scoped_thread &operator=(scoped_thread const &) = delete;
};

// struct func;

void f3()
{
    int some_local_state;
    scoped_thread t{ std::thread(func(some_local_state)) };
    do_something_in_current_thread();
}

class joining_thread
{
    std::thread t;

public:
    joining_thread() noexcept = default;

    template<typename Callable, typename... Args>
    explicit joining_thread(Callable &&func, Args &&...args)
        : t(std::forward<Callable>(func), std::forward<Args>(args)...)
    {
    }

    explicit joining_thread(std::thread t_) noexcept
        : t(std::move(t_))
    {
    }

    joining_thread(joining_thread &&other) noexcept
        : t(std::move(other.t))
    {
    }

    joining_thread &operator=(joining_thread &&other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other.t);
        return *this;
    }

    joining_thread &operator=(std::thread other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other);
        return *this;
    }

    ~joining_thread() noexcept
    {
        if (joinable())
            join();
    }

    void swap(joining_thread &other) noexcept { t.swap(other.t); }

    std::thread::id get_id() const noexcept { return t.get_id(); }

    bool joinable() const noexcept { return t.joinable(); }

    void join() { t.join(); }

    void detach() { t.detach(); }

    std::thread &as_thread() noexcept { return t; }

    const std::thread &as_thread() const noexcept { return t; }
};

void do_work(unsigned id) { }

// threads list
void f4()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.emplace_back(do_work, i);
    }
    for (auto &entry : threads)
        entry.join();
}

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread; // 向上取整的整数除法 (26 + 25 - 1)/ 25 = 2
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(),
                                 block_start,
                                 block_end,
                                 std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
    for (auto &entry : threads)
        entry.join();
    return std::accumulate(results.begin(), results.end(), init);
}

int main()
{
    // using function pointer
    //
    // thread t1(do_some_work);
    // t1.join();

    // using function object(any callable type)
    //
    // background_task f;
    // thread t2(f);
    // t2.join();
    // thread my_thread(background_task()); error: declares a my_thread function
    // thread t3((background_task()));
    // thread t3{background_task()};
    // t3.join();

    // using lambda
    //
    // thread t4([]() { cout << "lambda" << endl; });
    // t4.join();

    // oops();

    std::cout << std::thread::hardware_concurrency();

    return 0;
}
