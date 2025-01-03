#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "./include/array.hh"
#include "./include/graph.hh"
#include "./include/misc.hh"
#include "./include/tree.hh"
#include "./include/util.hh"

static void
print_divider_ (std::string str, char c = '-')
{
    int term_rows, term_cols;
    ext::get_winsz (&term_rows, &term_cols);

    int len = str.length ();

    std::cout << '\n'
              << std::string ((term_cols >> 1) - (len >> 1), c) << str
              << std::string (
                     term_cols - (term_cols >> 1) - (len - (len >> 1)), c)
              << '\n'
              << std::endl;
}

void
bfs (const std::vector<std::vector<int> > &adj, int start,
     std::vector<bool> *visited)
{
    // create a queue to visit nodes
    std::queue<int> to_visit;

    // enqueue the node and mark it as visited
    to_visit.push (start);
    (*visited)[start] = true;

    // iterate over the queue and visit nodes until it is empty
    while (!to_visit.empty ()) {
        // dequeue the node since it has been visited
        int node = to_visit.front ();
        to_visit.pop ();

        // do something with the node
        std::cout << node << ' ';

        // enqueue its unvisited neighbors
        for (int neighbor : adj[node]) {
            if (!(*visited)[neighbor]) {
                to_visit.push (neighbor);
                (*visited)[neighbor] = true;
            }
        }
    }
}

void
dfs (const std::vector<std::vector<int> > &adj, int node,
     std::vector<bool> *visited)
{
    if ((*visited)[node])
        return;
    (*visited)[node] = 1;

    // do something
    std::cout << node << " ";

    for (auto neighbor : adj[node])
        dfs (adj, neighbor, visited);
}

int
main ()
{
    print_divider_ ("USAGE DEMONSTRATION", '=');

    // ------ LONGEST INCR/DECR/NICR/NDCR SUBSEQUENCE ------ //

    print_divider_ ("LONGEST INCREASING SUBSEQUENCE");

    std::cout
        << "\nEnter the type of subsequence from the following options:\n"
           "\t0: INCREASING\n"
           "\t1: DECREASING\n"
           "\t2: NONDECREASING\n"
           "\t3: NONINCREASING\n"
           "> "
        << std::flush;

    int seqtype;
    std::cin >> seqtype;

    ext::arr::sqtype_e sqtype;
    bool               valid = true;

    switch (seqtype) {
        case 0:
            sqtype = ext::arr::INCR;
            break;
        case 1:
            sqtype = ext::arr::DECR;
            break;
        case 2:
            sqtype = ext::arr::NDCR;
            break;
        case 3:
            sqtype = ext::arr::NICR;
            break;
        default:
            std::cerr << "\033[31;1mfatal:\033[0m Invalid query type. "
                         "Continuing to next query."
                      << std::endl;
            valid = false;
            break;
    }

    if (valid) {
        std::cout << "Enter length of array:\n> " << std::flush;
        size_t len;
        std::cin >> len;

        std::vector<int> arr (len, 0);

        std::cout << "Enter array contents, separated by spaces:\n> "
                  << std::flush;

        for (size_t i = 0; i < len; ++i)
            std::cin >> arr[i];

        std::vector<int> lis = ext::arr::lis<int> (arr, sqtype);

        std::cout << "LIS: ";
        for (const int &num : lis)
            std::cout << num << ' ';
        std::cout << '\n';

        std::cout << "Length of LIS: " << lis.size () << '\n';

        if (len < 1024)
            std::cout << "Number of LIS: " << ext::arr::cnt_lis (arr)
                      << std::endl;
    }

    // ------ DFS and BFS ------ //

    int nodes, edges;

    print_divider_ ("DFS and BFS");

    std::cout << "input number of nodes and edges, separated by a space\n> "
              << std::flush;
    std::cin >> nodes >> edges;

    std::vector<std::vector<int> > adj (nodes, std::vector<int> ());

    std::cout << "input node connections, separated by a space\n";

    for (int i = 0; i < edges; ++i) {
        std::cout << "> " << std::flush;
        int n1, n2;
        std::cin >> n1 >> n2;
        adj[n1].push_back (n2);
        adj[n2].push_back (n1);
    }

    int start;
    std::cout << "input starting node\n> " << std::flush;
    std::cin >> start;

    std::vector<bool> visited (nodes, false);
    std::cout << "\n------DFS------\n";
    dfs (adj, start, &visited);
    std::cout << std::endl;

    visited.assign (nodes, false);
    std::cout << "\n------BFS------\n";
    bfs (adj, start, &visited);
    std::cout << std::endl;

    // ------ DIJKSTRA and ASTAR ------ //

    print_divider_ ("SHORTEST PATH");

    std::cout << "input number of nodes and edges, separated by a space\n> "
              << std::flush;
    std::cin >> nodes >> edges;

    ext::djk::graph djk_graph (nodes);
    ext::ast::graph ast_graph (nodes);

    int end;
    std::cout << "enter start and end, separated by a space:\n> "
              << std::flush;
    std::cin >> start >> end;

    std::cout << "input distances to start (ignored by dijkstra), separated "
                 "by a space:\n> "
              << std::flush;

    std::vector<int64_t> pdists (nodes, 0);

    for (int i = 0; i < nodes; ++i)
        std::cin >> pdists[i];

    std::cout << "input nodes and weight, separated by a space:\n";
    for (int i = 0; i < edges; ++i) {
        std::cout << "> " << std::flush;

        int n1, n2, weight;
        std::cin >> n1 >> n2 >> weight;

        djk_graph.add_edge_d (n1, n2, weight);
        ast_graph.add_edge_d (n1, n2, weight);
    }

    std::pair<int64_t, std::vector<size_t> > ans
        = djk_graph.traverse (start, end);

    print_divider_ ("DIJKSTRA");

    std::cout << "Weighted path length: " << ans.first << '\n';

    std::cout << "Path:\n";
    for (size_t i = 0; i < ans.second.size (); ++i) {
        std::cout << ans.second[i];
        if (i < ans.second.size () - 1)
            std::cout << " -> ";
    }

    ans = ast_graph.traverse (start, end, pdists);

    print_divider_ ("ASTAR");

    std::cout << "Weighted path length: " << ans.first << '\n';

    std::cout << "path:\n";
    for (size_t i = 0; i < ans.second.size (); ++i) {
        std::cout << ans.second[i];
        if (i < ans.second.size () - 1)
            std::cout << " -> ";
    }

    std::cout << '\n' << std::endl;

    // ------ SEGMENT TREE ------ //

    print_divider_ ("SEGMENT TREE");

    std::cout << "Enter array length:\n> " << std::flush;
    size_t len;
    std::cin >> len;

    ext::segtr::tree<int64_t> segtree (
        len, 0, [] (const int64_t &x, const int64_t &y) { return x + y; });

    std::cout << "Enter array contents, separated by spaces:\n> ";
    for (size_t i = 0; i < len; ++i) {
        int64_t x;
        std::cin >> x;
        segtree.set (i, x);
    }

    std::cout << "Enter number of queries:\n> " << std::flush;
    int queries;
    std::cin >> queries;

    while (queries--) {
        std::cout << "\nEnter the type of query from the following options:\n"
                     "\t0: SET\n"
                     "\t1: QUERY\n"
                     "> "
                  << std::flush;
        int qtype;
        std::cin >> qtype;

        switch (qtype) {
            case 0:
                size_t  pos;
                int64_t val;
                std::cout
                    << "Enter position and value, separated by a space:\n> "
                    << std::flush;
                std::cin >> pos >> val;

                // segtree is zero indexed so we decrement
                segtree.set (--pos, val);

                break;

            case 1:
                size_t l, r;
                std::cout << "Enter lower and upper bounds, separated by a "
                             "space:\n> "
                          << std::flush;
                std::cin >> l >> r;

                std::cout << segtree.query (--l, r) << std::endl;

                break;

            default:
                std::cerr << "\033[31;1mfatal:\033[0m Invalid query type. "
                             "Continuing to next query."
                          << std::endl;
                break;
        }
    }

    // ------ LAZY PROPAGATION SEGMENT TREE ------ //

    print_divider_ ("LAZY PROPAGATION SEGMENT TREE");

    std::cout << "Enter tree type from the following options:\n"
                 "\t0: SUM\n"
                 "\t1: MIN\n"
                 "\t2: MAX\n"
                 "> "
              << std::flush;
    int trtype;
    std::cin >> trtype;

    if (trtype >= 0 && trtype <= 2) {
        std::cout << "Enter array length:\n> " << std::flush;
        std::cin >> len;

        std::cout << "Enter array contents, separated by spaces:\n> ";
        std::vector<int64_t> vals (len, 0);
        for (size_t i = 0; i < len; ++i)
            std::cin >> vals[i];

        ext::lz_segtr::tree<int64_t> lz_segtree (
            len, 0, vals, (ext::lz_segtr::treeop_e)trtype);

        std::cout << "Enter number of queries:\n> " << std::flush;
        std::cin >> queries;

        while (queries--) {
            std::cout
                << "\nEnter the type of query from the following options:\n"
                   "\t0: ADD\n"
                   "\t1: SET\n"
                   "\t2: QUERY\n"
                   "> "
                << std::flush;
            int qtype;
            std::cin >> qtype;

            if (qtype == 0 || qtype == 1) {
                std::cout << "Enter lower and upper bounds and the value, "
                             "separated by a space:\n> "
                          << std::flush;
                size_t  l, r;
                int64_t val;
                std::cin >> l >> r >> val;

                // the segtree is ZERO INDEXED, so we decrement the bounds
                lz_segtree.upd (--l, --r, val, (ext::lz_segtr::qtype_e)qtype);
            } else if (qtype == 2) {
                std::cout << "Enter lower and upper bounds, separated by a "
                             "space:\n> "
                          << std::flush;
                size_t l, r;
                std::cin >> l >> r;

                std::cout << lz_segtree.query (--l, --r) << std::endl;
            }
        }
    } else {
        std::cerr << "\033[31;1mfatal:\033[0m Invalid tree type. Skipping."
                  << std::endl;
    }

    // ------ DISJOINT SET UNION (DSU) ------ //

    print_divider_ ("DISJOINT SET UNION");

    std::cout << "Enter the number of nodes:\n> " << std::flush;
    std::cin >> nodes;
    std::cout << "\nEnter the number of queries:\n> " << std::flush;
    std::cin >> queries;

    ext::dsu::disjoint_set<int> dsu (nodes);

    for (int i = 0; i < queries; ++i) {
        std::cout << "Enter the type of query from the following options:\n"
                     "\t0: ADD\n"
                     "\t1: CHECK\n"
                     "> "
                  << std::flush;
        int qtype;
        std::cin >> qtype;

        std::cout << "Enter the vertices, separated by a space:\n> "
                  << std::flush;
        int u, v;
        std::cin >> u >> v;
        if (qtype == 0)
            dsu.unite (u, v);
        else
            std::cout << (dsu.connected (u, v) ? "true" : "false") << '\n';
    }

    print_divider_ ("END", '=');

    return 0;
}
