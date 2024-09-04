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

int
main ()
{
    std::cout << "input nodes and edges, separated by a space\n> "
              << std::flush;
    int nodes, edges;
    std::cin >> nodes >> edges;
    std::cout << "input node connections, separated by a space\n";
    std::vector<std::vector<int> > adj (nodes, std::vector<int> ());
    for (int i = 0; i < edges; ++i) {
        std::cout << "> " << std::flush;
        int n1, n2;
        std::cin >> n1 >> n2;
        adj[n1].push_back (n2);
        adj[n2].push_back (n1);
    }

    std::vector<bool> visited (nodes, false);
    std::cout << "bfs from 0:\n";
    bfs (adj, 0, &visited);

    return 0;
}
