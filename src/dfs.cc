#include <iostream>
#include <vector>

int nodes = 4;
int edges = 4;
std::vector<int> visited;

void
dfs (std::vector<std::vector<int> > adj, int node) {
    if (visited[node - 1])
        return;
    visited[node - 1] = 1;
    std::cout << node << " ";
    for (auto neighbor : adj[node - 1]) {
        dfs (adj, neighbor);
    }
}

int
main () {
    std::cout << "input nodes and edges" << std::endl;
    std::cin >> nodes >> edges;
    std::vector<std::vector<int> > adj;
    adj.assign (nodes, std::vector<int> ());
    visited.assign (nodes, 0);
    int node1 = 0;
    int node2 = 1;
    std::cout << "input connections" << std::endl;
    for (size_t i = 0; i < edges; ++i) {
        std::cin >> node1 >> node2;
        adj[node1 - 1].push_back (node2);
        adj[node2 - 1].push_back (node1);
    }
    dfs (adj, adj[0][0]);

    std::cout << std::endl;
    return 0;
}
