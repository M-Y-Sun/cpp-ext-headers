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

template <typename T> class ast_graph final
{
private:
    // ------ STRUCTS ------ //

    struct node_t_ {
        size_t   id;
        node_t_ *prev;
        T        wdist; // weighted distance
        T        pdist; // physical distance

        node_t_ &
        operator++ ()
        {
            ++this->id;
            return *this;
        }
    };

    struct nbr_t_ {
        T        weight;
        node_t_ *node;
    };

    // ------ MEMBER VARIABLES ------ //

    static constexpr const T MAXVAL_ = std::numeric_limits<T> ().max ();

    // stores all defined nodes that will be updated in calls to astar
    std::vector<node_t_> nodes_;

    // adjacency list representing the graph
    std::vector<std::vector<nbr_t_> > adj_;

    // visited array
    std::vector<bool> visited_;

    // ------ PRIVATE FUNCTIONS ------ //

    /** Backtracks edges until the start node.
     * @return The path to the end node. */
    std::vector<size_t>
    backtrack_ (node_t_ end)
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
                   node_t_ ({ 0, nullptr, MAXVAL_, MAXVAL_ }));
    }

    /** Sets up the class instance for running astar. */
    void
    setup_ (size_t start, const std::vector<T> &pdists)
    {
        assert (pdists.size () == nodes_.size ());

        reset_nodes_ ();

        visited_.assign (nodes_.size (), false);

        for (size_t i = 0; i < nodes_.size (); ++i)
            nodes_[i].pdist = pdists[i];

        nodes_[start].wdist = 0;
    }

public:
    ast_graph (size_t len)
        : nodes_ (len, node_t_ ()), adj_ (len, std::vector<nbr_t_> ())
    {
        reset_nodes_ ();
    }

    /** Adds an undirected edge. */
    void
    add_edge_ud (size_t n1_id, size_t n2_id, T weight)
    {
        adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
    }

    /** Adds a directed edge. */
    void
    add_edge_d (size_t n1_id, size_t n2_id, T weight)
    {
        adj_[n1_id].push_back ({ weight, &nodes_[n2_id] });
        adj_[n2_id].push_back ({ weight, &nodes_[n1_id] });
    }

    /** @param `start`: The start node.
     * @param `end`: The target "ending" node.
     * @param `pdists`: A vector containing the physical distances between the
     * start node and the other nodes.
     * @return The weighted path length and a vector of the indices of each
     * node in the path. */
    std::pair<T, std::vector<size_t> >
    astar (size_t start, size_t end, const std::vector<T> &pdists)
    {
        setup_ (start, pdists);

        auto node_gt = [] (const node_t_ lhs, const node_t_ rhs) {
            // compare using an additonal physical distance heuristic
            return lhs.wdist + lhs.pdist > rhs.wdist + rhs.pdist;
        };

        std::priority_queue<node_t_, std::vector<node_t_>, decltype (node_gt)>
            pq;
        pq.push (nodes_[start]);

        while (!pq.empty ()) {
            node_t_ cur = pq.top ();
            pq.pop ();

            if (cur.id == end)
                break;

            if (cur.wdist != nodes_[cur.id].wdist)
                continue;

            for (nbr_t_ &next : adj_[cur.id]) {
                T wdistval = cur.wdist + next.weight;

                if (next.node->wdist > wdistval) {
                    next.node->wdist = wdistval;
                    next.node->prev  = &nodes_[cur.id];
                    pq.push (*next.node);
                }
            }
        }

        std::vector<size_t> path = backtrack_ (nodes_[end]);
        T                   plen = nodes_[end].wdist;

        if (plen == MAXVAL_) {
            plen = -1;
            path.clear ();
        }

        return { plen, path };
    }
};

} // namespace ext

int
main ()
{
    int n, e;
    std::cout << "enter number of nodes and edges, separated by a space:\n> "
              << std::flush;
    std::cin >> n >> e;

    ext::ast_graph<int64_t> graph (n);

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
