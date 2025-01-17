// 引用计数的一个实现例子
// QQmlRefCount 子类化，结合引用计数指针QQmlRefPointer一起使用
// usage:
// class Q_QML_PRIVATE_EXPORT QQmlOpenMetaObjectType : public QQmlRefCount 
// QQmlRefPointer<QQmlOpenMetaObjectType> type; 

#include <QtCore/qatomic.h>
class Q_QML_PRIVATE_EXPORT QQmlRefCount // 只能通过继承来使用
{
    Q_DISABLE_COPY_MOVE(QQmlRefCount)
public:
    inline QQmlRefCount();
    inline void addref() const;
    inline void release() const;
    inline int count() const;

protected:
  // 声明protected的目的是禁止在栈中产生对象，只能通过继承来构造子类（构造/析构为public）
    inline virtual ~QQmlRefCount();

private:
    mutable QAtomicInt refCount;
};

QQmlRefCount::QQmlRefCount()
: refCount(1)
{
}

QQmlRefCount::~QQmlRefCount()
{
    Q_ASSERT(refCount.loadRelaxed() == 0);
}

void QQmlRefCount::addref() const
{
    Q_ASSERT(refCount.loadRelaxed() > 0);
    refCount.ref();
}

void QQmlRefCount::release() const
{
    Q_ASSERT(refCount.loadRelaxed() > 0);
    if (!refCount.deref())
        delete this;
}

int QQmlRefCount::count() const
{
    return refCount.loadRelaxed();
}

template<class T>
class QQmlRefPointer
{
public:
    enum Mode {
        AddRef,
        Adopt
    };
    inline QQmlRefPointer();
    inline QQmlRefPointer(T *, Mode m = AddRef);
    inline QQmlRefPointer(const QQmlRefPointer<T> &);
    inline QQmlRefPointer(QQmlRefPointer<T> &&);
    inline ~QQmlRefPointer();

    inline QQmlRefPointer<T> &operator=(const QQmlRefPointer<T> &o);
    inline QQmlRefPointer<T> &operator=(QQmlRefPointer<T> &&o);

    inline bool isNull() const { return !o; }

    inline T* operator->() const { return o; }
    inline T& operator*() const { return *o; }
    explicit inline operator bool() const { return o != nullptr; }
    inline T* data() const { return o; }

    inline QQmlRefPointer<T> &adopt(T *);

    inline T* take() { T *res = o; o = nullptr; return res; }

private:
    T *o;
};

template<class T>
QQmlRefPointer<T>::QQmlRefPointer()
: o(nullptr)
{
}

template<class T>
QQmlRefPointer<T>::QQmlRefPointer(T *o, Mode m)
: o(o)
{
    if (m == AddRef && o)
        o->addref();
}

template<class T>
QQmlRefPointer<T>::QQmlRefPointer(const QQmlRefPointer<T> &other)
: o(other.o)
{
    if (o) o->addref();
}

template <class T>
QQmlRefPointer<T>::QQmlRefPointer(QQmlRefPointer<T> &&other)
    : o(other.take())
{
}

template<class T>
QQmlRefPointer<T>::~QQmlRefPointer()
{
    if (o) o->release();
}

template<class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::operator=(const QQmlRefPointer<T> &other)
{
    if (other.o) other.o->addref();
    if (o) o->release();
    o = other.o;
    return *this;
}

template <class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::operator=(QQmlRefPointer<T> &&other)
{
    QQmlRefPointer<T> m(std::move(other));
    qSwap(o, m.o);
    return *this;
}

/*!
Takes ownership of \a other.  take() does *not* add a reference, as it assumes ownership
of the callers reference of other.
*/
template<class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::adopt(T *other)
{
    if (o) o->release();
    o = other;
    return *this;
}



