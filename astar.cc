#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

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
astar (std::vector<std::vector<nbr_t> > *adj, size_t start, size_t end,
       std::vector<node_t> *nodes)
{
    auto node_gt = [] (const node_t lhs, const node_t rhs) {
        // compare using an additonal physical distance heuristic
        return lhs.wdist + lhs.pdist > rhs.wdist + rhs.pdist;
    };

    std::priority_queue<node_t, std::vector<node_t>, decltype (node_gt)> pq;
    pq.push ((*nodes)[start]);

    while (!pq.empty ()) {
        node_t cur = pq.top ();
        pq.pop ();

        if (cur.id == end)
            break;

        if (cur.wdist != (*nodes)[cur.id].wdist)
            continue;

        for (nbr_t &next : (*adj)[cur.id]) {
            int64_t wdistval = cur.wdist + next.weight;

            if (next.node->wdist > wdistval) {
                next.node->wdist = wdistval;
                next.node->prev  = &(*nodes)[cur.id];
                pq.push (*next.node);
            }
        }
    }

    std::vector<size_t> path = backtrack_ ((*nodes)[end]);
    int64_t             plen = (*nodes)[end].wdist;

    if (plen == INT64_MAX) {
        plen = -1;
        path.clear ();
    }

    return { plen, path };
}

int
main ()
{
    // freopen ("astar.in", "r", stdin);

    int n, e;
    std::cout << "enter number of nodes and edges, separated by a space:\n> "
              << std::flush;
    std::cin >> n >> e;

    // stores all the defined nodes
    std::vector<node_t> nodes (n);
    std::iota (nodes.begin (), nodes.end (),
               node_t ({ 0, nullptr, INT_MAX, INT_MAX }));

    std::vector<std::vector<nbr_t> > adj (n, std::vector<nbr_t> ());
    std::vector<bool>                visited (n, false);

    int start, end;
    std::cout << "enter start and end, separated by a space:\n> "
              << std::flush;
    std::cin >> start >> end;
    nodes[start].wdist = 0;

    std::cout << "input distances to start, separated by a space:\n> "
              << std::flush;
    for (int i = 0; i < n; ++i)
        std::cin >> nodes[i].pdist;

    std::cout << "input nodes and weight, separated by a space:\n";
    for (int i = 0; i < e; ++i) {
        std::cout << "> " << std::flush;

        int n1, n2, weight;
        std::cin >> n1 >> n2 >> weight;
        adj[n1].push_back ({ weight, &nodes[n2] });
        adj[n2].push_back ({ weight, &nodes[n1] });
    }

    std::pair<int64_t, std::vector<size_t> > ans
        = astar (&adj, start, end, &nodes);

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
