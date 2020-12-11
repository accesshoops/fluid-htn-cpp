// 
// Define aliases and wrappers for STL types here so that custom implementations can override them (UE4 for example).
//
#pragma once

#if !USING_CUSTOM_STL

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T, class... ARGS>
SharedPtr<T> MakeSharedPtr(ARGS&&... args)
{
    return std::make_shared<T>(std::forward<ARGS>(args)...);
}
template <typename T>
using EnableSharedFromThis = std::enable_shared_from_this<T>;

#define SharedFromThis()  shared_from_this()

template <typename T,typename U>
SharedPtr<T> StaticCastPtr(const SharedPtr<U>& Other)
{
    return std::static_pointer_cast<T>(Other);
}

template<typename T>
using ArrayType = std::vector<T>;

template<typename T>
class Queue
{
private:
    std::queue<T> q;

public:
    void   push(const T& x) { q.push(x); }
    void   push(T&& x) { q.push(std::move(x)); }
    void   pop() { q.pop(); }
    T&     front() { return q.front(); }
    size_t size() { return q.size(); }
    bool   empty() { return q.empty(); }
    void   clear() { q = std::queue<T>(); }
};
template<typename T>
class Stack
{
private:
    std::stack<T> s;

public:
    void   push(const T& x) { s.push(x); }
    void   push(T&& x) { s.push(std::move(x)); }
    void   pop() { return s.pop(); }
    T&     top() { return s.top(); }
    size_t size() { return s.size(); }
    bool   empty() { return s.empty(); }
    void   clear() { s = std::stack<T>(); }
};

template<typename T,typename U>
class Map
{
private:
    std::unordered_map<T, U> m;
public:
    template<typename V>
    auto Insert(V&& x) -> decltype(m.insert(std::forward<V>(x))) { return m.insert(std::forward<V>(x)); }

    auto Find(T x) -> decltype(m.find(std::forward<T>(x))) { return m.find(std::forward<T>(x)); }

    auto Contains(T x) { return (m.find(x) != m.end()); }
};
template<typename T>
class Set
{
private:
    std::unordered_set<T> s;
public:
    template<typename V>
    auto Insert(V&& x) -> decltype(s.insert(std::forward<V>(x))) { return s.insert(std::forward<V>(x)); }

    auto Find(T x) -> decltype(s.find(std::forward<T>(x))) { return s.find(std::forward<T>(x)); }

    auto End() { return s.end(); }
};

template<typename T, typename U>
auto MakePair(T&& A, U&& B) -> decltype(std::make_pair(std::forward<T>(A), std::forward<U>(B)) )
{
    return std::make_pair(std::forward<T>(A), std::forward<U>(B));
}

template<typename P1, typename P2>
class Pair
{
private:
    std::pair<P1,P2> p;

public:
    Pair(P1 X, P2 Y): p(X,Y){}
    P1& First() { return p.first; }
    P2& Second() { return p.second; }

};


#ifndef FHTN_FATAL_EXCEPTION
#define FHTN_FATAL_EXCEPTION(condition, msg)                                                                                          \
    if (!(condition))                                                                                                              \
    {                                                                                                                              \
        throw std::exception(msg);                                                                                                \
    }

#endif

#ifndef FHTN_FATAL_EXCEPTION_V
#define FHTN_FATAL_EXCEPTION_V(condition, fmt, ...)  this is for UE4 checkf, verifymsg etc. do not t use elsewhere
#endif

#else // USING_CUSTOM_STL
#include <CoreMinimal.h>
#include <Containers/Queue.h>

template<typename T>
using SharedPtr = TSharedPtr<T>;

template<typename T, class... ARGS>
SharedPtr<T> MakeSharedPtr(ARGS&&... args)
{
    return MakeShared<T>(std::forward<ARGS>(args)...);
}
template <typename T>
using EnableSharedFromThis = TSharedFromThis<T>;

#define SharedFromThis()  SharedThis(this)

template <typename T,typename U>
SharedPtr<T> StaticCastPtr(const SharedPtr<U>& Other)
{
    return StaticCastSharedPtr<T>(Other);
}

template<typename T>
using ArrayType = TArray<T>;
#define push_back Add
#define size() Num()
#define clear() Empty()
#define pop_back Pop

template<typename T>
class Queue
{
private:
    TArray<T> q;

public:
    void push(const T& x)
    {
        q.Push(x);
    }
    void push(T&& x)
    {
        q.Push(std::move(x));
    }
    void pop()
    {
        q.Pop();
    }
    T&     front() { return q.Top(); }
    size_t size() { return q.Num(); }
    bool   empty() { return (q.Num() == 0); }
    void   clear() { q = TArray<T>(); }
};
template<typename T>
class Stack
{
private:
    TArray<T> s;

public:
    void   push(const T& x) { s.Push(x); }
    void   push(T&& x) { s.Push(std::move(x)); }
    void   pop() { return s.Pop(); }
    T&     top() { return s.Top(); }
    size_t size() { return s.Num(); }
    bool   empty() { return sNum() == 0; }
    void   clear() { s.Empty(); }
};

template<typename T,typename U>
class Map
{
private:
    TMap<T, U> m;
public:

    struct MapEntry
    {
        T first;
        U second;
    };
    TSharedPtr<MapEntry> Find(T key)
    {
        U* val = m.Find(key);
        if (val)
        {
            return MakeShared<MapEntry>(MapEntry{key, *val});
        }
        return nullptr;
    }
    auto Insert(T x, U y) { return m.Add(x,y); }

    auto Contains(T x) { return m.Contains(x); }
};

template<typename T>
class Set
{
private:
    TSet<T> s;
public:
    template<typename V>
    auto Insert(V&& x) -> decltype(s.Add(std::forward<V>(x))) { return s.Add(std::forward<V>(x)); }

    auto Find(T x) -> decltype(s.Find(std::forward<T>(x))) { return s.Find(std::forward<T>(x)); }

    auto Contains(T x) { return s.Contains(x); }
};

#define MakePair(a,b) a,b

template<typename P1, typename P2>
class Pair
{
private:
    TPair<P1,P2> p;

public:
    Pair(P1 X, P2 Y): p(X,Y){}
    P1& First() { return p.Key; }
    P2& Second() { return p.Value; }

};


#endif //! USING_CUSTOM_STL

using StringType = std::string;

template <typename T>
StringType ToString(const T& arg)
{
    return std::to_string(arg);
}

#define FHTN_FATAL_EXCEPTION(condition, msg) checkf(condition, TEXT(msg))
