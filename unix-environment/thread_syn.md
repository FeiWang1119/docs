# 线程同步

当多个线程共享相同的内存时，需要确保每个线程看到一致的数据视图。

## 互斥量

互斥量本质上是一把锁，在访问共享资源前对互斥量进行设置（加锁），在访问完成后释放（解锁）互斥量。

初始化：

- PTHREAD_MUTEX_INITIALIZER (只适用于静态分配的互斥量)
- pthread_mutex_init函数

初始化和销毁：

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                       const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

加锁和解锁：

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
                            const struct timespec *restrict abstime);
```
pthread_mutex_timedlock 达到超时时间时, 不会对互斥量进行加锁而是返回错误玛 ETIMEDOUT

死锁两种情况：

 - 线程对同一个互斥量加锁两次
 - 两个线程相互请求另一个线程拥有的资源

 第二种死锁情况解决方法:

 - 控制加锁的顺序,总是以相同的顺序加锁
 - 使用pthread_mutex_trylock,返回成功可以前进，返回失败释放已经占有的锁,过一段时间再试

## 读写锁

与互斥量类似，但允许更高的并行性。适用于对数据结构读的次数远大于写的情况.

两点： 

- 读共享，写独占
- 当读写同时到达时，写优先级高(避免读模式锁长期占用，而写模式锁一直得不到满足)

初始化和销毁：

```c
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
                        const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

加锁和解锁：

```c
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

带有超时的读写锁：

```c
int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
                               const struct timespec *restrict abstime);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock,
                               const struct timespec *restrict abstime);
```

## 条件变量

与互斥量一起使用，允许线程以无竞争的方式等待特定的条件发生。

条件本身是由互斥量保护的。线程在改变条件状态之前必须首先锁住互斥量。其他线程在获得互斥量之前不会察觉到这种改变。因为互斥量必须在锁定以后才能计算条件。

初始化和销毁：

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pthread_cond_init(pthread_cond_t *restrict cond,
                      const pthread_condattr_t *restrict attr);
int pthread_cond_destroy(pthread_cond_t *cond);
```

等待条件满足：

```c
int pthread_cond_wait(pthread_cond_t *restrict cond,
                      pthread_mutex_t *restrict mutex);
int pthread_cond_timedwait(pthread_cond_t *restrict cond,
                           pthread_mutex_t *restrict mutex,
                           const struct timespec *restrict abstime);
```

传递给 `pthread_cond_wait` 的互斥量对条件进行保护。调用者把锁住的互斥量传给函数，函数然后自动把调用线程放到等待条件的线程列表上，对互斥量解锁。这就关闭了条件检查和线程进入休眠状态等待条件改变这两个操作之间的时间通道，这样线程就不会错过条件的任何变化。`pthread_cond_wait` 返回时，互斥量再次被锁住。

从`pthread_cond_wait` 或者 `pthread_cond_timedwait` 调用成功返回时，线程需要重新计算条件，因为另一个线程可能己经在运行并改变了条件。

通知：

```c
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```

这两个函数可以用于通知线程条件已经满足。`pthread_cond_signal` 函数至少能唤醒等待该条件的线程，而 `pthread_cond_broadcast` 函数则能唤醒等待该条件的所有线程。

## 自旋锁

自旋锁与互斥量类似，但它不是通过休眠使进程阻塞，而是在获取锁之前一直处于忙等（自旋）阻塞状态。自旋锁可用于以下情况：锁被持有的时间短，而且线程并不希望在重新调度上花费太多的成本。

初始化和销毁:

```c
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_destroy(pthread_spinlock_t *lock);
```

加锁和解锁：

```c
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);
```
## 屏障

屏障（barrier）是用户协调多个线程并行工作的同步机制。屏障允许每个线程等待，直到所有的合作线程都到达某一点，然后从该点继续执行。我们已经看到一种屏障，pthread_join函数就是一种屏障，允许一个线程等待，直到另一个线程退出。

初始化和销毁:

```c
int pthread_barrier_init(pthread_barrier_t *restrict barrier,
                         const pthread_barrierattr_t *restrict attr, unsigned count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

等待：

```c
int pthread_barrier_wait(pthread_barrier_t *barrier);
```

