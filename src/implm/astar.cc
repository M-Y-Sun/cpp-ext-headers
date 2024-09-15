#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

#include "../include/graph.hh"

namespace ext
{

namespace ast
{

// stores all defined nodes that will be updated in calls to astar
std::vector<node_t> nodes_;

// adjacency list representing the graph
std::vector<std::vector<edge_t> > adj_;

// visited array
std::vector<bool> visited_;

/** Backtracks edges until the start node.
 * @return int64_the path to the end node. */
std::vector<size_t>
graph::backtrack_ (node_t end)
{
    std::vector<size_t> path;

    while (end.prev != nullptr) {
        path.push_back (end.id);
        end = *end.prev;
    }

    path.push_back (end.id);
    std::reverse (path.begin (), path.end ());

    return path;
}

/** Initializes the nodes_ array with increasing id */
inline void
graph::reset_nodes_ ()
{
    std::iota (nodes_.begin (), nodes_.end (),
               node_t ({ 0, nullptr, INT64_MAX, INT64_MAX }));
}

/** Sets up the class instance for running astar. */
void
graph::setup_ (size_t start, const std::vector<int64_t> &pdists)
{
    assert (pdists.size () == nodes_.size ());

    reset_nodes_ ();

    visited_.assign (nodes_.size (), false);

    for (size_t i = 0; i < nodes_.size (); ++i)
        nodes_[i].pdist = pdists[i];

    nodes_[start].wdist = 0;
}

graph::graph (size_t len)
    : nodes_ (len, node_t ()), adj_ (len, std::vector<edge_t> ())
{
    reset_nodes_ ();
}

/** Adds an undirected edge. */
void
graph::add_edge_ud (size_t n1_id, size_t n2_id, int64_t weight)
{
    adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
}

/** Adds a directed edge. */
void
graph::add_edge_d (size_t n1_id, size_t n2_id, int64_t weight)
{
    adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
    adj_[n2_id].push_back ({ weight, &nodes_[n1_id] });
}

/** @param `start`: int64_the start node.
 * @param `end`: int64_the target "ending" node.
 * @param `pdists`: A vector containing the physical distances between the
 * start node and the other nodes.
 * @return int64_the weighted path length and a vector of the indices of
 * each node in the path. */
std::pair<int64_t, std::vector<size_t> >
graph::traverse (size_t start, size_t end, const std::vector<int64_t> &pdists)
{
    setup_ (start, pdists);

    auto node_gt = [] (const node_t lhs, const node_t rhs) {
        // compare using an additonal physical distance heuristic
        return lhs.wdist + lhs.pdist > rhs.wdist + rhs.pdist;
    };

    std::priority_queue<node_t, std::vector<node_t>, decltype (node_gt)> pq;
    pq.push (nodes_[start]);

    while (!pq.empty ()) {
        node_t cur = pq.top ();
        pq.pop ();

        if (cur.id == end)
            break;

        if (cur.wdist != nodes_[cur.id].wdist)
            continue;

        for (edge_t &next : adj_[cur.id]) {
            int64_t wdistval = cur.wdist + next.weight;

            if (next.dest->wdist > wdistval) {
                next.dest->wdist = wdistval;
                next.dest->prev  = &nodes_[cur.id];
                pq.push (*next.dest);
            }
        }
    }

    std::vector<size_t> path = backtrack_ (nodes_[end]);
    int64_t             plen = nodes_[end].wdist;

    if (plen == INT64_MAX) {
        plen = -1;
        path.clear ();
    }

    return { plen, path };
}

} // namespace ast

} // namespace ext
