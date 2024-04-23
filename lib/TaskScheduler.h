#pragma once

#include "FutureResult.h"
#include "Task.h"
#include "DependenciesGraph.h"
#include "CustomConcepts.h"
#include <vector>

class TTaskScheduler {
public:
    TTaskScheduler() {}

    ~TTaskScheduler() {}

    template <CopyConstructible T>
    FutureResult<T> getFutureResult(size_t idx) {
        dependencies.AddEdge(idx, dependencies.size());
        FutureResult<T> future_result(tasks[idx]);
        return future_result;
    }

    template <CopyConstructible T>
    const T& getResult(size_t idx) const {
        if (!tasks[idx]->IsDone()) {
            std::vector<size_t> execute_these_tasks = dependencies.TopSort(idx);
            for (auto other_idx: execute_these_tasks) {
                tasks[other_idx]->Invoke();
            }
        }

        return tasks[idx]->GetResult().Get<T>();
    }

    template <FunctorWithZeroArgs Functor>
    size_t add(const Functor& functor) {
        std::shared_ptr<Task> ptr = std::make_shared<TaskWithZeroArgs<Functor>>(functor);
        tasks.push_back(ptr);
        dependencies.AddVertex(tasks.size());
        return tasks.size() - 1;
    }

    template <CopyConstructible U, FunctorWithOneArg<U> Functor>
    size_t add(const Functor& functor, const U& u) {
        std::shared_ptr<Task> ptr = std::make_shared<TaskWithOneArg<U, Functor>>(functor, u);
        tasks.push_back(ptr);
        dependencies.AddVertex(tasks.size());
        return tasks.size() - 1;
    }

    template <CopyConstructible U, CopyConstructible V, FunctorWithTwoArgs<U, V> Functor>
    size_t add(const Functor& functor, const U& u, const V& v) {
        std::shared_ptr<Task> ptr = std::make_shared<TaskWithTwoArgs<U, V, Functor>>(functor, u, v);
        tasks.push_back(ptr);
        dependencies.AddVertex(tasks.size());
        return tasks.size() - 1;
    }

    void executeAll() const {
        for (auto task: tasks) {
            task->Invoke();
        }
    }

private:
    std::vector<std::shared_ptr<Task>> tasks;
    DependenciesGraph dependencies;
};
