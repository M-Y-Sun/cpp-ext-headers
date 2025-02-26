#pragma once
#ifndef GRAPH_HH
#define GRAPH_HH

#include <utility>
#include <vector>

namespace ext
{

namespace djk
{

// graph node
struct node_t {
    size_t  id;
    node_t *prev;
    int64_t dist;

    // for std::iota
    node_t &
    operator++ ()
    {
        ++this->id;
        return *this;
    }
};

// node connection (for adjacency list)
struct edge_t {
    int64_t weight;
    node_t *dest;
};

class graph final
{
    // stores all defined nodes that will be updated in calls to astar
    std::vector<node_t> nodes_;

    // adjacency list representing the graph
    std::vector<std::vector<edge_t> > adj_;

    // visited array
    std::vector<bool> visited_;

    /** Backtracks edges until the start node.
     * @return int64_the path to the end node. */
    std::vector<size_t> backtrack_ (node_t end);

    /** Initializes the nodes_ array with increasing id */
    inline void reset_nodes_ ();

    /** Set up class instance for dijkstra traversal */
    inline void setup_ (size_t start);

public:
    graph (size_t len);

    /** Adds an undirected edge. */
    void add_edge_ud (size_t n1_id, size_t n2_id, int64_t weight);

    /** Adds a directed edge. */
    void add_edge_d (size_t n1_id, size_t n2_id, int64_t weight);

    /** @param `start`: int64_the start node.
     * @param `end`: int64_the target "ending" node.
     * @return int64_the weighted path length and a vector of the indices
     * of each node in the path. */
    std::pair<int64_t, std::vector<size_t> > traverse (size_t start,
                                                       size_t end);
};

} // namespace djk

namespace ast
{

// graph node
struct node_t {
    size_t  id;
    node_t *prev;
    int64_t wdist; // weighted distance
    int64_t pdist; // physical distance

    node_t &
    operator++ ()
    {
        ++this->id;
        return *this;
    }
};

// node connection (for adjacency list)
struct edge_t {
    int64_t weight;
    node_t *dest;
};

class graph final
{
    // stores all defined nodes that will be updated in calls to astar
    std::vector<node_t> nodes_;

    // adjacency list representing the graph
    std::vector<std::vector<edge_t> > adj_;

    // visited array
    std::vector<bool> visited_;

    /** Backtracks edges until the start node.
     * @return int64_the path to the end node. */
    std::vector<size_t> backtrack_ (node_t end);

    /** Initializes the nodes_ array with increasing id */
    inline void reset_nodes_ ();

    /** Sets up the class instance for running astar. */
    void setup_ (size_t start, const std::vector<int64_t> &pdists);

public:
    graph (size_t len);

    /** Adds an undirected edge. */
    void add_edge_ud (size_t n1_id, size_t n2_id, int64_t weight);

    /** Adds a directed edge. */
    void add_edge_d (size_t n1_id, size_t n2_id, int64_t weight);

    /** @param `start`: int64_the start node.
     * @param `end`: int64_the target "ending" node.
     * @param `pdists`: A vector containing the physical distances between the
     * start node and the other nodes.
     * @return int64_the weighted path length and a vector of the indices of
     * each node in the path. */
    std::pair<int64_t, std::vector<size_t> >
    traverse (size_t start, size_t end, const std::vector<int64_t> &pdists);
};

} // namespace ast

namespace dsu
{

template <typename T> class disjoint_set final
{
private:
    std::vector<T>      parents_;
    std::vector<size_t> sizes_;

public:
    disjoint_set (size_t sz);

    /**
     * Recursively finds the representative node of x's component, visiting the
     * immediate parent every iteration.
     *
     * Optimized by path compression to reduce tree height by raising the
     * parent on each iteration of find.
     *
     * @return The representative node in x's component.
     * */
    T find (T x);

    /**
     * Merges two components optimized with union by size to work in O(log N).
     * @return If the merge changed connectivity.
     */
    bool unite (T x, T y);

    /** @return If x and y are in the same connected component */
    bool connected (T x, T y);
};

} // namespace dsu

} // namespace ext

#endif
