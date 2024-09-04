#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

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

// backtrack the path using the end node and pointer to previous node
static std::vector<size_t>
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

// returns the weighted path length and a vector of the indices of each node in
// the path
std::pair<int64_t, std::vector<size_t> >
dijkstra (std::vector<std::vector<nbr_t> > *adj, size_t start, size_t end,
          std::vector<node_t> *nodes)
{
    // priority queue comparator to sort greatest
    auto node_gt = [] (const node_t lhs, const node_t rhs) {
        return lhs.dist > rhs.dist;
    };

    // stores values instead of pointers for the coexistence of updated nodes
    std::priority_queue<node_t, std::vector<node_t>, decltype (node_gt)> pq;

    // push the starting node onto the pq
    pq.push ((*nodes)[start]);

    // traverse until there is nothing left to visit
    while (!pq.empty ()) {
        // visit the current node; store it at the top and pop it from the pq
        node_t cur = pq.top ();
        pq.pop ();

        // if we visited the end, we are done; break
        if (cur.id == end)
            break;

        // if the current distance is different from the final distance stored
        // in the nodes array, it has already been visited since a different
        // distance means we have arrived from a different path. if the path
        // happens to have the same distance than it doesnt matter so we save
        // time and skip over it.
        if (cur.dist != (*nodes)[cur.id].dist)
            continue;

        // queue each neighbor of the current node and update values if needed
        for (nbr_t &next : (*adj)[cur.id]) {
            // new distance is cur distance + weight to next node
            int64_t distval = cur.dist + next.weight;

            // if distance is smaller, update the distance and previous node
            if (next.node->dist > distval) {
                next.node->dist = distval;

                // we can directly assign addresses here because the place that
                // adj and nodes points are passed as pointer parameters so
                // they should exist even after the function returns
                next.node->prev = &(*nodes)[cur.id];
                pq.push (*next.node);
            }
        }
    }

    std::vector<size_t> path = backtrack_ ((*nodes)[end]);
    int64_t             plen = (*nodes)[end].dist;

    // if the weighted length is still INT64_MAX, it means it was not updated
    // due to there being no path from the start to the end. in this case,
    // return -1 as the length and clear the path vector
    if (plen == INT64_MAX) {
        plen = -1;
        path.clear ();
    }

    return { plen, path };
}

int
main ()
{
    size_t n, e;
    std::cout << "enter number of nodes and edges, separated by a space:\n> "
              << std::flush;
    std::cin >> n >> e;

    // stores all the defined nodes
    std::vector<node_t> nodes (n, node_t ());
    std::iota (nodes.begin (), nodes.end (), node_t ({ 0, nullptr, INT_MAX }));

    std::vector<std::vector<nbr_t> > adj (n, std::vector<nbr_t> ());

    size_t start, end;
    std::cout << "enter start and end, separated by a space:\n> "
              << std::flush;
    std::cin >> start >> end;
    // --start;
    // --end;
    nodes[start].dist = 0;

    std::cout << "input nodes and weight, separated by a space:\n";
    for (size_t i = 0; i < e; ++i) {
        std::cout << "> " << std::flush;

        int64_t n1, n2, weight;
        std::cin >> n1 >> n2 >> weight;
        // --n1;
        // --n2;
        adj[n1].push_back ({ weight, &nodes[n2] });
        adj[n2].push_back ({ weight, &nodes[n1] });
    }

    std::pair<int64_t, std::vector<size_t> > ans
        = dijkstra (&adj, start, end, &nodes);

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
