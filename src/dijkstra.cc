#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
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
struct nbr_t {
    int64_t weight;
    node_t *node;
};

class graph final
{
private:
    static constexpr const int64_t MAXVAL_
        = std::numeric_limits<int64_t> ().max ();

    // stores all defined nodes that will be updated in calls to astar
    std::vector<node_t> nodes_;

    // adjacency list representing the graph
    std::vector<std::vector<nbr_t> > adj_;

    // visited array
    std::vector<bool> visited_;

    /** Backtracks edges until the start node.
     * @return int64_the path to the end node. */
    std::vector<size_t>
    backtrack_ (node_t end)
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
    reset_nodes_ ()
    {
        std::iota (nodes_.begin (), nodes_.end (),
                   node_t ({ 0, nullptr, MAXVAL_ }));
    }

    /** Set up class instance for dijkstra traversal */
    inline void
    setup_ (size_t start)
    {
        reset_nodes_ ();
        visited_.assign (nodes_.size (), false);
        nodes_[start].dist = 0;
    }

public:
    graph (size_t len)
        : nodes_ (len, node_t ()), adj_ (len, std::vector<nbr_t> ())
    {
        reset_nodes_ ();
    }

    /** Adds an undirected edge. */
    void
    add_edge_ud (size_t n1_id, size_t n2_id, int64_t weight)
    {
        adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
    }

    /** Adds a directed edge. */
    void
    add_edge_d (size_t n1_id, size_t n2_id, int64_t weight)
    {
        adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
        adj_[n2_id].push_back ({ weight, &nodes_[n1_id] });
    }

    /** @param `start`: int64_the start node.
     * @param `end`: int64_the target "ending" node.
     * @return int64_the weighted path length and a vector of the indices of
     * each node in the path. */
    std::pair<int64_t, std::vector<size_t> >
    dijkstra (size_t start, size_t end)
    {
        setup_ (start);

        // priority queue comparator to sort greatest
        auto node_gt = [] (const node_t lhs, const node_t rhs) {
            return lhs.dist > rhs.dist;
        };

        // stores values instead of pointers for the coexistence of updated
        // nodes
        std::priority_queue<node_t, std::vector<node_t>, decltype (node_gt)>
            pq;

        // push the starting node onto the pq
        pq.push (nodes_[start]);

        // traverse until there is nothing left to visit
        while (!pq.empty ()) {
            // visit the current node; store it at the top and pop it from the
            // pq
            node_t cur = pq.top ();
            pq.pop ();

            // if we visited the end, we are done; break
            if (cur.id == end)
                break;

            // if the current distance is different from the final distance
            // stored in the nodes array, it has already been visited since a
            // different distance means we have arrived from a different path.
            // if the path happens to have the same distance than it doesnt
            // matter so we save time and skip over it.
            if (cur.dist != nodes_[cur.id].dist)
                continue;

            // queue each neighbor of the current node and update values if
            // needed
            for (nbr_t &next : adj_[cur.id]) {
                // new distance is cur distance + weight to next node
                int64_t distval = cur.dist + next.weight;

                // if distance is smaller, update the distance and previous
                // node
                if (next.node->dist > distval) {
                    next.node->dist = distval;

                    // we can directly assign addresses here because the place
                    // that adj and nodes points are passed as pointer
                    // parameters so they should exist even after the function
                    // returns
                    next.node->prev = &nodes_[cur.id];
                    pq.push (*next.node);
                }
            }
        }

        std::vector<size_t> path = backtrack_ (nodes_[end]);
        int64_t             plen = nodes_[end].dist;

        // if the weighted length is still INint64_t64_MAX, it means it was not
        // updated due to there being no path from the start to the end. in
        // this case, return -1 as the length and clear the path vector
        if (plen == MAXVAL_) {
            plen = -1;
            path.clear ();
        }

        return { plen, path };
    }
};

} // namespace djk

} // namespace ext

int
main ()
{
    size_t n, e;
    std::cout << "enter number of nodes and edges, separated by a space:\n> "
              << std::flush;
    std::cin >> n >> e;

    ext::djk::graph graph (n);

    size_t start, end;
    std::cout << "enter start and end, separated by a space:\n> "
              << std::flush;
    std::cin >> start >> end;

    std::cout << "input nodes and weight, separated by a space:\n";
    for (size_t i = 0; i < e; ++i) {
        std::cout << "> " << std::flush;

        int64_t n1, n2, weight;
        std::cin >> n1 >> n2 >> weight;
        graph.add_edge_d (n1, n2, weight);
    }

    std::pair<int64_t, std::vector<size_t> > ans = graph.dijkstra (start, end);

    std::cout << "\n--------\nweighted path length: " << ans.first << '\n';

    std::cout << "path:\n";
    for (size_t i = 0; i < ans.second.size (); ++i) {
        std::cout << ans.second[i];
        if (i < ans.second.size () - 1)
            std::cout << " -> ";
    }

    std::cout << '\n';

    return 0;
}
