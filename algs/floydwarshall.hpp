#include <vector>
#include <limits>
std::vector<std::vector<int>> floydWarshall(std::vector<std::vector<int>> &edges, int n)
{
    // assumes edges are [from, to, weight]
    // construct dp
    std::vector<std::vector<int>> dp = std::vector<std::vector<int>>(n, std::vector<int>(n, std::numeric_limits<int>::max()));
    for (std::vector<int> &edge : edges)
    {
        dp[edge[0]][edge[1]] = edge[2];
    }

    // floyd warshall
    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k][j]);
            }
        }
    }

    return dp;
}