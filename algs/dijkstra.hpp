#include <vector>
#include <queue>
#include <limits>
std::vector<int> dijkstra(std::vector<std::vector<int>> &edges, int n, int start)
{
    // assumes edges are [from, to, weight]
    // construct adjacency list
    std::vector<std::vector<std::pair<int, int>>> adj = std::vector<std::vector<std::pair<int, int>>>(n, std::vector<std::pair<int, int>>());
    for (int i = 0; i < edges.size(); i++)
    {
        adj[edges[i][0]].push_back({edges[i][1], edges[i][2]});
    }

    // initialize heap
    // min heap sorted by weight (second item)
    auto comp = [](std::pair<int, int> a, std::pair<int, int> b)
    { return a.second > b.second; };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> pq(comp);

    // initialize distances
    pq.push({start, 0});
    for (int i = 0; i < n; i++)
    {
        if (i != start)
        {
            pq.push({i, std::numeric_limits<int>::max()});
        }
    }
    std::vector<int> dist = std::vector<int>(n, std::numeric_limits<int>::max());
    dist[start] = 0;

    // dijkstra
    while (!pq.empty())
    {
        std::pair<int, int> cur = pq.top();
        pq.pop();

        // see if this is an unoptimal item
        if (dist[cur.first] < cur.second)
        {
            // cur.second is the weight, meaning this is an unoptimal item
            continue;
        }

        // optimal item, so explore neighbors
        for (std::pair<int, int> neighbor : adj[cur.first])
        {
            // check if this makes a better path
            if (dist[neighbor.first] > dist[cur.first] + neighbor.second)
            {
                // if so, update distance and push to heap
                dist[neighbor.first] = dist[cur.first] + neighbor.second;
                pq.push({neighbor.first, dist[neighbor.first]});
            }
        }
    }
    return dist; // distances from node start to all nodes
}