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
