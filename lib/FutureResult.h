#pragma once

#include <memory>
#include "Task.h"
#include "CustomConcepts.h"

template <CopyConstructible T>
class FutureResult {
public:
    FutureResult(std::shared_ptr<Task> ptr): ptr(ptr) {}

    operator const T&() const {
        return ptr->GetResult().Get<T>();
    }
private:
    std::shared_ptr<Task> ptr;
};