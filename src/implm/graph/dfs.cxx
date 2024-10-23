#include <iostream>
#include <vector>

void
dfs (const std::vector<std::vector<int> > &adj, int node,
     std::vector<bool> *visited)
{
    (*visited)[node] = true;

    // do something with node
    std::cout << node << " ";

    for (auto neighbor : adj[node])
        if (!(*visited)[node])
            dfs (adj, neighbor, visited);
}
