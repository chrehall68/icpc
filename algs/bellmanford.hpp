#include <vector>
#include <limits>

std::vector<int> bellmanFord(std::vector<std::vector<int>> &edges, int n, int start)
{
    // assumes edges are [from, to, weight]
    std::vector<int> dist = std::vector<int>(n, std::numeric_limits<int>::max());
    dist[start] = 0;

    // now relax up to n-1 times; if we relax more than n-1 times, we have a negative cycle
    bool changed = true;
    int i = 0;
    while (changed && i < n - 1)
    {
        changed = false;
        for (std::vector<int> &edge : edges)
        {
            // relax edge if possible
            if (dist[edge[0]] != std::numeric_limits<int>::max() && dist[edge[1]] > dist[edge[0]] + edge[2])
            {
                dist[edge[1]] = dist[edge[0]] + edge[2];
                changed = true;
            }
        }
        ++i;
    }
    if (changed)
    {
        // negative cycle
        // populate with infinite distance
        changed = true;
        while (changed)
        {
            for (std::vector<int> &edge : edges)
            {
                // if an edge relaxation is possible, then this is part of a negative cycle
                // so flag with infinite
                if (dist[edge[0]] != std::numeric_limits<int>::min() &&
                    dist[edge[0]] != std::numeric_limits<int>::max() &&
                    dist[edge[1]] > dist[edge[0]] + edge[2])
                {
                    dist[edge[1]] = std::numeric_limits<int>::min();
                    changed = true;
                }
            }
        }
    }

    return dist;
}