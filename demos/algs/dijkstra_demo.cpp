// this file contains an example implementation of dijkstra's algorithm
#include <iostream>
#include "algs/dijkstra.hpp"
using namespace std;

int main()
{
    vector<vector<int>> edges = {{0, 1, 2}, {1, 0, 3}, {0, 2, 10}, {1, 2, 5}, {2, 0, 1}, {3, 1, 2}, {2, 3, 11}};
    int n = 4;
    int start = 0;
    vector<int> dist = dijkstra(edges, n, start);
    for (int i = 0; i < dist.size(); i++)
    {
        cout << start << " -> " << i << " = " << dist[i] << endl;
    }
    return 0;
}