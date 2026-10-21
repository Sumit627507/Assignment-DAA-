#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;          // (time, node)
const int INF = INT_MAX;

struct Road { int to, time; };

vector<vector<Road>> city;
int N;

// Dijkstra with a min-heap; fills dist[] and prev[]
void shortestTimes(int src, vector<int>& dist, vector<int>& prev) {
    dist.assign(N, INF);
    prev.assign(N, -1);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;               // stale entry
        for (Road &r : city[u]) {
            int nd = d + r.time;
            if (nd < dist[r.to]) {
                dist[r.to] = nd;
                prev[r.to] = u;
                pq.push({nd, r.to});
            }
        }
    }
}

string route(int t, const vector<int>& prev) {
    vector<int> p;
    for (int v = t; v != -1; v = prev[v]) p.push_back(v);
    reverse(p.begin(), p.end());
    string s;
    for (size_t i = 0; i < p.size(); i++) {
        s += char('A' + p[i]);
        if (i + 1 < p.size()) s += " -> ";
    }
    return s;
}

void report(int src, const vector<int>& hosp) {
    vector<int> dist, prev;
    shortestTimes(src, dist, prev);
    int best = -1;
    for (int h : hosp) {
        if (dist[h] == INF) cout << "Hospital " << char('A'+h) << " : unreachable\n";
        else cout << "Hospital " << char('A'+h) << " : " << dist[h] << " minutes\n";
        if (dist[h] != INF && (best == -1 || dist[h] < dist[best])) best = h;
    }
    if (best == -1) { cout << "\nNo hospital can be reached.\n"; return; }
    cout << "\nClosest hospital: " << char('A'+best) << " (" << dist[best] << " minutes)\n";
    cout << "Route: " << route(best, prev) << "\n";
}

int main() {
    int E, src, H;
    cout << "Number of junctions: ";            cin >> N;
    cout << "Number of roads: ";                cin >> E;
    city.assign(N, {});
    cout << "Roads (from to minutes):\n";
    for (int i = 0; i < E; i++) { int u, v, w; cin >> u >> v >> w; city[u].push_back({v, w}); }
    cout << "Ambulance location: ";             cin >> src;
    cout << "Number of hospitals: ";            cin >> H;
    vector<int> hosp(H);
    cout << "Hospital junctions: ";             for (int &h : hosp) cin >> h;

    cout << "\n--- Before traffic change ---\n";
    report(src, hosp);

    int u, v, w;
    cout << "\nTraffic alert! Road to update (from to): "; cin >> u >> v;
    cout << "New travel time: ";                           cin >> w;
    for (Road &r : city[u]) if (r.to == v) r.time = w;

    cout << "\n--- After traffic change ---\n";
    report(src, hosp);
    return 0;
}
