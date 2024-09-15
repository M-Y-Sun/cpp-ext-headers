#include <iostream>
#include <queue>
#include <vector>

#include "./include/graph.hh"
#include "./include/tree.hh"

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
    int nodes, edges;

    // ------ DFS and BFS ------ //

    std::cout << "------DFS and BFS------\n";
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

    std::cout << "------SHORTEST PATH------\n";
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

    std::cout << "\n------DIJKSTRA------\nWeighted path length: " << ans.first
              << '\n';

    std::cout << "Path:\n";
    for (size_t i = 0; i < ans.second.size (); ++i) {
        std::cout << ans.second[i];
        if (i < ans.second.size () - 1)
            std::cout << " -> ";
    }

    ans = ast_graph.traverse (start, end, pdists);

    std::cout << "\n------ASTAR------\nWeighted path length: " << ans.first
              << '\n';

    std::cout << "path:\n";
    for (size_t i = 0; i < ans.second.size (); ++i) {
        std::cout << ans.second[i];
        if (i < ans.second.size () - 1)
            std::cout << " -> ";
    }

    std::cout << '\n';

    return 0;
}
