#pragma once

#include <vector>

class DependenciesGraph {
public:
    DependenciesGraph() {}

    void dfs(size_t u, std::vector<bool>& marked, std::vector<size_t>& top_sort) const {
        marked[u] = true;
        for (auto i: Graph[u]) {
            if (!marked[i]) {
                dfs(i, marked, top_sort);
            }
        }
        top_sort.push_back(u);
    }

    void AddVertex(size_t n) {
        if (n > Graph.size()) {
            Graph.resize(n);
        }
    }

    void AddEdge(size_t u, size_t depends_on_u) {
        if (depends_on_u + 1 > Graph.size()) {
            Graph.resize(depends_on_u + 1);
        }
        Graph[depends_on_u].push_back(u);
    }

    std::vector<size_t> TopSort(size_t u) const {
        std::vector<size_t> top_sort;
        std::vector<bool> marked(Graph.size(), false);
        dfs(u, marked, top_sort);
        return top_sort;
    }

    size_t size() const {
        return Graph.size();
    }

private:
    std::vector<std::vector<size_t>> Graph;
};