// DAA Assignment 5 : Multistage Graph - SwiftCargo delivery route optimisation
// Technique : Dynamic Programming (backward, stage by stage)
#include <bits/stdc++.h>
using namespace std;
 
const int INF = 1e9;
 
struct Edge { int to, w; };
 
class MultistageGraph {
public:
    int n;                              // total nodes (numbered stage by stage)
    vector<int> stage;                  // stage number of every node
    vector<string> name;                // display name of every node
    vector<vector<Edge>> adj;           // adj[u] = edges u -> v (v is in next stage)
    vector<int> cost, nxt;              // cost[u] = best cost u -> sink, nxt[u] = next hop
 
    MultistageGraph(int n) : n(n), stage(n), name(n), adj(n), cost(n, INF), nxt(n, -1) {}
 
    void addEdge(int u, int v, int w) {
        if (stage[v] != stage[u] + 1) {         // multistage property check
            cerr << "Invalid edge " << name[u] << "->" << name[v] << "\n";
            return;
        }
        adj[u].push_back({v, w});
    }
 
    // Recompute cost[] / nxt[] for nodes 'from' down to 0 (nodes are numbered by stage,
    // so going from a high index to a low index is a backward stage-by-stage sweep).
    void solve(int from) {
        if (from == n - 1) { cost[n - 1] = 0; from = n - 2; }
        for (int i = from; i >= 0; i--) {
            cost[i] = INF; nxt[i] = -1;
            for (auto &e : adj[i])
                if (cost[e.to] != INF && e.w + cost[e.to] < cost[i]) {
                    cost[i] = e.w + cost[e.to];
                    nxt[i] = e.to;
                }
        }
    }
 
    // Real-time change: new travel time (INF = road closed) and incremental re-computation
    void updateEdge(int u, int v, int newW) {
        for (auto &e : adj[u]) if (e.to == v) e.w = newW;
        solve(u);                               // only nodes at or before u can change
    }
 
    string path(int s) {
        if (cost[s] == INF) return "No route";
        string p = name[s];
        for (int i = s; nxt[i] != -1; i = nxt[i]) p += " -> " + name[nxt[i]];
        return p;
    }
};
 
MultistageGraph buildSwiftCargo() {
    MultistageGraph g(10);
    const char *nm[10] = {"S", "A", "B", "C", "D", "E", "F", "G", "H", "T"};
    int st[10] = {1, 2, 2, 2, 3, 3, 3, 4, 4, 5};
    for (int i = 0; i < 10; i++) { g.name[i] = nm[i]; g.stage[i] = st[i]; }
    // node ids: S0 A1 B2 C3 D4 E5 F6 G7 H8 T9
    int E[][3] = {{0,1,2},{0,2,6},{0,3,3},
                  {1,4,4},{1,5,7},{2,4,1},{2,5,3},{2,6,6},{3,5,2},{3,6,5},
                  {4,7,6},{4,8,3},{5,7,2},{5,8,7},{6,7,4},{6,8,3},
                  {7,9,5},{8,9,4}};
    for (auto &e : E) g.addEdge(e[0], e[1], e[2]);
    return g;
}
 
void printAll(MultistageGraph &g) {
    for (int i = g.n - 2; i >= 0; i--)
        cout << "  cost(" << g.name[i] << ") = " << setw(2) << g.cost[i]
             << "   next = " << (g.nxt[i] < 0 ? "-" : g.name[g.nxt[i]]) << "\n";
}
 
int main() {
    MultistageGraph g = buildSwiftCargo();
    cout << "SwiftCargo Multistage Graph: " << g.n << " nodes, 5 stages\n";
 
    g.solve(g.n - 1);
    cout << "\n--- Backward DP table (hours to reach T) ---\n";
    printAll(g);
    cout << "\nMinimum delivery time S -> T : " << g.cost[0] << " hours\n";
    cout << "Optimal route               : " << g.path(0) << "\n";
 
    // ---- Batch processing: many requests answered from ONE DP pass ----
    cout << "\n--- Batch delivery requests (start node -> T) ---\n";
    int reqs[] = {0, 1, 2, 3, 6};
    for (int r : reqs)
        cout << "  From " << g.name[r] << " : " << setw(2) << g.cost[r]
             << " hrs   " << g.path(r) << "\n";
 
    // ---- Real-time event 1: traffic jam on E -> G ----
    cout << "\nTraffic alert! Road E -> G delayed: new time = 9 hours\n";
    g.updateEdge(5, 7, 9);
    cout << "--- After traffic change ---\n";
    cout << "Minimum delivery time S -> T : " << g.cost[0] << " hours\n";
    cout << "Optimal route               : " << g.path(0) << "\n";
 
    // ---- Real-time event 2: road closure D -> H ----
    cout << "\nRoad closed! D -> H is blocked\n";
    g.updateEdge(4, 8, INF);
    cout << "--- After road closure ---\n";
    cout << "Minimum delivery time S -> T : " << g.cost[0] << " hours\n";
    cout << "Optimal route               : " << g.path(0) << "\n";
 
    // ---- Scalability test: 500 stages x 4 nodes = 2002 nodes ----
    int K = 500, W = 4, N = K * W + 2;
    MultistageGraph big(N);
    mt19937 rng(42);
    big.name[0] = "S"; big.stage[0] = 1;
    for (int k = 0; k < K; k++)
        for (int j = 0; j < W; j++) {
            int id = 1 + k * W + j;
            big.name[id] = "n" + to_string(id); big.stage[id] = k + 2;
        }
    big.name[N - 1] = "T"; big.stage[N - 1] = K + 2;
    for (int j = 0; j < W; j++) big.addEdge(0, 1 + j, rng() % 20 + 1);
    for (int k = 0; k + 1 < K; k++)
        for (int a = 0; a < W; a++)
            for (int b = 0; b < W; b++)
                big.addEdge(1 + k * W + a, 1 + (k + 1) * W + b, rng() % 20 + 1);
    for (int j = 0; j < W; j++) big.addEdge(1 + (K - 1) * W + j, N - 1, rng() % 20 + 1);
 
    int edges = 0; for (auto &v : big.adj) edges += v.size();
    auto t0 = chrono::high_resolution_clock::now();
    big.solve(N - 1);
    auto t1 = chrono::high_resolution_clock::now();
    cout << "\n--- Scalability test ---\n";
    cout << "Nodes = " << N << ", Edges = " << edges << ", Stages = " << K + 2 << "\n";
    cout << "Minimum cost S -> T = " << big.cost[0] << "\n";
    cout << "Time taken = " << chrono::duration<double, milli>(t1 - t0).count() << " ms\n";
    return 0;
}
