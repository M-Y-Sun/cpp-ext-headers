#include <iostream>
#include <vector>

void
dfs (const std::vector<std::vector<int> > &adj, int node,
     std::vector<bool> *visited)
{
    if ((*visited)[node])
        return;
    (*visited)[node] = 1;

    // do something with node
    std::cout << node << " ";

    for (auto neighbor : adj[node])
        dfs (adj, neighbor, visited);
}

// int
// main ()
// {
//     int nodes, edges;
//
//     std::cout << "input nodes and edges" << std::endl;
//     std::cin >> nodes >> edges;
//
//     std::vector<std::vector<int> > adj (nodes, std::vector<int> ());
//     std::vector<bool>              visited (nodes, 0);
//     int                            node1 = 0;
//     int                            node2 = 1;
//
//     std::cout << "input connections" << std::endl;
//
//     for (int i = 0; i < edges; ++i) {
//         std::cin >> node1 >> node2;
//         adj[node1].push_back (node2);
//         adj[node2].push_back (node1);
//     }
//
//     dfs (adj, adj[0][0], &visited);
//
//     std::cout << std::endl;
//     return 0;
// }
