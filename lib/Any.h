#pragma once

#include "CustomConcepts.h"

class any {
public:
    any(): ptr(nullptr) {}

    any(const any& other): ptr(other.ptr->GetCopy()) {}

    any(any&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    template <CopyConstructible T>
    any(T&& value): ptr(new Derived<T>(value)) {}

    any& operator=(const any& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr->GetCopy();
        }

        return *this;
    }

    any& operator=(any&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        return *this;
    }

    template <CopyConstructible T>
    any& operator=(T&& value) {
        delete ptr;
        ptr = new Derived<T>(value);

        return *this;
    }

    ~any() {
        delete ptr;
    };

    template <CopyConstructible T>
    const T& Get() const  {
        auto* pointer = dynamic_cast<Derived<T>*>(ptr);
        if (!pointer) {
            throw std::bad_cast();
        }
        return pointer->value;
    }

private:
    struct Base {
        virtual Base* GetCopy() const = 0;
        virtual ~Base() {};
    };

    template <CopyConstructible T>
    struct Derived: public Base {
        T value;

        Derived(const T& value): value(value) {}

        ~Derived() {};

        Base* GetCopy() const override {
            return new Derived(value);
        }
    };

    Base* ptr;
};
