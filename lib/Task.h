#pragma once

#include <utility>
#include <memory>
#include "Any.h"
#include "CustomConcepts.h"

class Task {
public:
    Task() {};
    virtual const any& GetResult() = 0;
    virtual bool IsDone() = 0;
    virtual void Invoke() = 0;
};

template <FunctorWithZeroArgs Functor>
class TaskWithZeroArgs: public Task {
public:
    TaskWithZeroArgs(const Functor& functor): functor_(functor), already_done(false) {}

    const any& GetResult() override {
        return result;
    }

    bool IsDone() override {
        return already_done;
    }

    void Invoke() override {
        if (!already_done) {
            already_done = true;
            result = functor_();
        }
    }

private:
    bool already_done;
    Functor functor_;
    any result;
};

template <CopyConstructible U, FunctorWithOneArg<U> Functor>
class TaskWithOneArg: public Task {
public:
    TaskWithOneArg(const Functor& functor, const U& u): functor_(functor), u_(u), already_done(false) {}

    const any& GetResult() override {
        return result;
    }

    bool IsDone() override {
        return already_done;
    }

    void Invoke() override {
        if (!already_done) {
            already_done = true;
            result = functor_(u_);
        }
    }

private:
    bool already_done;
    Functor functor_;
    U u_;
    any result;
};

template <CopyConstructible U, CopyConstructible V, FunctorWithTwoArgs<U, V> Functor>
class TaskWithTwoArgs: public Task {
public:
    using return_type = decltype(std::declval<Functor>()(std::declval<U>(), std::declval<V>()));
    TaskWithTwoArgs(const Functor& functor, const U& u, const V& v): functor_(functor), u_(u), v_(v), already_done(false) {}

    const any& GetResult() override {
        return result;
    }

    bool IsDone() override {
        return already_done;
    }

    void Invoke() override {
        if (!already_done) {
            already_done = true;
            result = functor_(u_, v_);
        }
    }

private:
    bool already_done;
    Functor functor_;
    U u_;
    V v_;
    any result;
};
