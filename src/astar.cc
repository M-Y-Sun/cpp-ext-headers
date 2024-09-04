#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

namespace ext
{

namespace ast
{

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
                   node_t ({ 0, nullptr, MAXVAL_, MAXVAL_ }));
    }

    /** Sets up the class instance for running astar. */
    void
    setup_ (size_t start, const std::vector<int64_t> &pdists)
    {
        assert (pdists.size () == nodes_.size ());

        reset_nodes_ ();

        visited_.assign (nodes_.size (), false);

        for (size_t i = 0; i < nodes_.size (); ++i)
            nodes_[i].pdist = pdists[i];

        nodes_[start].wdist = 0;
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
     * @param `pdists`: A vector containing the physical distances between the
     * start node and the other nodes.
     * @return int64_the weighted path length and a vector of the indices of
     * each node in the path. */
    std::pair<int64_t, std::vector<size_t> >
    astar (size_t start, size_t end, const std::vector<int64_t> &pdists)
    {
        setup_ (start, pdists);

        auto node_gt = [] (const node_t lhs, const node_t rhs) {
            // compare using an additonal physical distance heuristic
            return lhs.wdist + lhs.pdist > rhs.wdist + rhs.pdist;
        };

        std::priority_queue<node_t, std::vector<node_t>, decltype (node_gt)>
            pq;
        pq.push (nodes_[start]);

        while (!pq.empty ()) {
            node_t cur = pq.top ();
            pq.pop ();

            if (cur.id == end)
                break;

            if (cur.wdist != nodes_[cur.id].wdist)
                continue;

            for (nbr_t &next : adj_[cur.id]) {
                int64_t wdistval = cur.wdist + next.weight;

                if (next.node->wdist > wdistval) {
                    next.node->wdist = wdistval;
                    next.node->prev  = &nodes_[cur.id];
                    pq.push (*next.node);
                }
            }
        }

        std::vector<size_t> path = backtrack_ (nodes_[end]);
        int64_t             plen = nodes_[end].wdist;

        if (plen == MAXVAL_) {
            plen = -1;
            path.clear ();
        }

        return { plen, path };
    }
};

} // namespace ast

} // namespace ext

int
main ()
{
    int n, e;
    std::cout << "enter number of nodes and edges, separated by a space:\n> "
              << std::flush;
    std::cin >> n >> e;

    ext::ast::graph graph (n);

    int start, end;
    std::cout << "enter start and end, separated by a space:\n> "
              << std::flush;
    std::cin >> start >> end;

    std::cout << "input distances to start, separated by a space:\n> "
              << std::flush;
    std::vector<int64_t> pdists (n, 0);
    for (int i = 0; i < n; ++i)
        std::cin >> pdists[i];

    std::cout << "input nodes and weight, separated by a space:\n";
    for (int i = 0; i < e; ++i) {
        std::cout << "> " << std::flush;

        int n1, n2, weight;
        std::cin >> n1 >> n2 >> weight;
        graph.add_edge_d (n1, n2, weight);
    }

    std::pair<int64_t, std::vector<size_t> > ans
        = graph.astar (start, end, pdists);

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
