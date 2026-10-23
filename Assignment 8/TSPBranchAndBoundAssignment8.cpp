```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

const int N = 4;
const int INF = 1000000000;

const string CITY[N] = {"A", "B", "C", "D"};

const int COST[N][N] = {
    {INF, 10, 15, 20},
    {10, INF, 35, 25},
    {15, 35, INF, 30},
    {20, 25, 30, INF}
};

struct Node {
    vector<vector<int>> matrix;
    int bound;
    int level;
    int current;
    int pathCost;
    vector<int> path;

    Node(vector<vector<int>> matrix, int bound,
         int level, int current, int pathCost,
         vector<int> path) {

        this->matrix = matrix;
        this->bound = bound;
        this->level = level;
        this->current = current;
        this->pathCost = pathCost;
        this->path = path;
    }
};

// Comparator for least-cost priority queue.
struct Compare {
    bool operator()(const Node& a, const Node& b) const {
        if (a.bound != b.bound) {
            return a.bound > b.bound;
        }

        return a.path > b.path;
    }
};

// Convert the cost matrix into a vector of vectors.
vector<vector<int>> copyMatrix() {
    vector<vector<int>> matrix(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = COST[i][j];
        }
    }

    return matrix;
}

// Reduce every row and column and return the reduction cost.
int reduceMatrix(vector<vector<int>>& matrix) {
    int reduction = 0;

    // Row reduction.
    for (int i = 0; i < N; i++) {
        int minimum = INF;

        for (int j = 0; j < N; j++) {
            minimum = min(minimum, matrix[i][j]);
        }

        if (minimum != INF && minimum > 0) {
            reduction += minimum;

            for (int j = 0; j < N; j++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minimum;
                }
            }
        }
    }

    // Column reduction.
    for (int j = 0; j < N; j++) {
        int minimum = INF;

        for (int i = 0; i < N; i++) {
            minimum = min(minimum, matrix[i][j]);
        }

        if (minimum != INF && minimum > 0) {
            reduction += minimum;

            for (int i = 0; i < N; i++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minimum;
                }
            }
        }
    }

    return reduction;
}

// Check whether a city is already in the path.
bool contains(const vector<int>& path, int city) {
    return find(path.begin(), path.end(), city) != path.end();
}

// Format the tour as A -> B -> C -> D -> A.
string formatPath(const vector<int>& path) {
    string result;

    for (int i = 0; i < (int)path.size(); i++) {
        if (i > 0) {
            result += " -> ";
        }

        result += CITY[path[i]];
    }

    return result;
}

int main() {
    cout << "ASSIGNMENT NO. 8: TSP USING LC BRANCH AND BOUND\n";
    cout << "------------------------------------------------\n";
    cout << "Cities: A, B, C, D\n";
    cout << "Cost matrix (INF on diagonal):\n";

    cout << "       A    B    C    D\n";

    for (int i = 0; i < N; i++) {
        cout << CITY[i] << "  ";

        for (int j = 0; j < N; j++) {
            if (COST[i][j] == INF) {
                cout << setw(4) << "INF" << " ";
            } else {
                cout << setw(4) << COST[i][j] << " ";
            }
        }

        cout << '\n';
    }

    // Create the root node.
    vector<vector<int>> rootMatrix = copyMatrix();
    int rootBound = reduceMatrix(rootMatrix);

    vector<int> rootPath;
    rootPath.push_back(0);

    Node root(rootMatrix, rootBound, 0, 0, 0, rootPath);

    // Min-priority queue: lowest bound is expanded first.
    priority_queue<Node, vector<Node>, Compare> pq;
    pq.push(root);

    int bestCost = INF;
    vector<int> bestPath;

    int expanded = 0;
    int pruned = 0;

    // LC Branch and Bound search.
    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();

        // Prune nodes whose lower bound cannot improve the solution.
        if (node.bound >= bestCost) {
            pruned++;
            continue;
        }

        expanded++;

        // All cities visited: return to the starting city.
        if (node.level == N - 1) {
            int completedCost =
                node.pathCost + COST[node.current][0];

            if (completedCost < bestCost) {
                bestCost = completedCost;
                bestPath = node.path;
                bestPath.push_back(0);
            }

            continue;
        }

        int u = node.current;

        // Generate child nodes for unvisited cities.
        for (int v = 0; v < N; v++) {
            if (contains(node.path, v) || COST[u][v] == INF) {
                continue;
            }

            vector<vector<int>> childMatrix = node.matrix;

            // No further departure from the current city.
            for (int k = 0; k < N; k++) {
                childMatrix[u][k] = INF;
            }

            // The next city already has its incoming edge.
            for (int k = 0; k < N; k++) {
                childMatrix[k][v] = INF;
            }

            // Prevent returning to the start prematurely.
            // Preserve the return edge for the final city.
            if (node.level + 1 < N - 1) {
                childMatrix[v][0] = INF;
            }

            int reductionCost = reduceMatrix(childMatrix);

            int childBound = node.bound
                           + node.matrix[u][v]
                           + reductionCost;

            // Bound-based pruning.
            if (childBound >= bestCost) {
                pruned++;
                continue;
            }

            vector<int> childPath = node.path;
            childPath.push_back(v);

            Node child(
                childMatrix,
                childBound,
                node.level + 1,
                v,
                node.pathCost + COST[u][v],
                childPath
            );

            pq.push(child);
        }
    }

    // Display the final result.
    cout << '\n';

    if (bestPath.empty()) {
        cout << "Optimal tour : No tour found\n";
        cout << "Minimum cost : N/A\n";
    } else {
        cout << "Optimal tour : " << formatPath(bestPath) << '\n';
        cout << "Minimum cost : " << bestCost << '\n';
    }

    cout << "Expanded nodes: " << expanded << '\n';
    cout << "Pruned nodes  : " << pruned << '\n';
    cout << "Result        : Optimal tour found using LC Branch and Bound.\n";

    return 0;
}
```
