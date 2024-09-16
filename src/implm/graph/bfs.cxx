#include <iostream>
#include <queue>
#include <vector>

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

    std::cout << std::endl;
}
