#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

//This is for trim helpin
static inline string ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
    return s;
}
static inline string rtrim(string s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
    return s;
}
static inline string trim(string s) { return rtrim(ltrim(s)); }

// This is the Graph:  map city name -> index, adjacency list of indices
vector<string> names;
unordered_map<string,int> idx;
vector<vector<int>> adj;

// add city and return index
int get_index(const string &name) {
    auto it = idx.find(name);
    if (it != idx.end()) return it->second;
    int id = (int)names.size();
    names.push_back(name);
    idx[name] = id;
    adj.emplace_back();
    return id;
}

// This is for giving roles for flight.txt. 
bool parse_file(const string &filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Cannot open " << filename << endl;
        return false;
    }
    string line;
    int current = -1;
    bool expectTo = false;
    while (getline(fin, line)) {
        string s = trim(line);
        if (s.empty()) { expectTo = false; continue; }
        if (s.rfind("From:", 0) == 0) {
            string city = trim(s.substr(5));
            current = get_index(city);
            expectTo = false;
        } else if (s.rfind("To", 0) == 0) {
            // find colon
            auto pos = s.find(':');
            if (pos != string::npos) {
                string rest = trim(s.substr(pos+1));
                if (!rest.empty()) {
                    int id = get_index(rest);
                    if (current >= 0) adj[current].push_back(id);
                }
            }
            expectTo = true;
        } else if (expectTo) {
            // continuation destination line
            string city = s;
            if (!city.empty()) {
                int id = get_index(city);
                if (current >= 0) adj[current].push_back(id);
            }
        }
    }
    fin.close();
    return true;
}

// BFS returning path vector of indices. Empty vector means that is is unreachable
vector<int> bfs_path(int src, int dst) {
    if (src < 0 || dst < 0) return {};
    int n = (int)names.size();
    vector<int> parent(n, -1);
    vector<int> dist(n, -1);
    queue<int> q;
    q.push(src); dist[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == dst) break;
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    if (dist[dst] == -1) return {};
    vector<int> path;
    for (int cur = dst; cur != -1; cur = parent[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    return path;
}

void print_path_indices(const vector<int>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << names[path[i]];
        if (i + 1 < path.size()) cout << " to ";
    }
    cout << "\n";
}

// Question 1
void q1() {
    cout << "Enter source city (exactly as in file):\n> ";
    string A; getline(cin, A); A = trim(A);
    cout << "Enter destination city:\n> ";
    string B; getline(cin, B); B = trim(B);
    cout << "Enter maximum allowed connections (integer):\n> ";
    string tmp; getline(cin, tmp); int maxconn = stoi(trim(tmp));
    if (idx.find(A) == idx.end() || idx.find(B) == idx.end()) { cout << "One or both cities not found.\n"; return; }
    int a = idx[A], b = idx[B];
    auto path = bfs_path(a,b);
    if (path.empty()) { cout << "No route from " << A << " to " << B << "\n"; return; }
    int connections = (int)path.size() - 1;
    if (connections > maxconn) {
        cout << "No route with less than or equal to " << maxconn << " connections (shortest has " << connections << ").\n";
        return;
    }
    print_path_indices(path);
    cout << "total connection: " << connections << "\n";
}


// Helper to join two paths where p1 end == p2 start
vector<int> join_paths(const vector<int>& p1, const vector<int>& p2) {
    if (p1.empty() || p2.empty()) return {};
    if (p1.back() != p2.front()) return {};
    vector<int> out = p1;
    out.insert(out.end(), p2.begin()+1, p2.end());
    return out;
}

// Question 2: A -> D through B and C
void q2() {
    cout << "Enter city A (start):\n> "; string A; getline(cin, A); A = trim(A);
    cout << "Enter city B (must visit):\n> "; string B; getline(cin, B); B = trim(B);
    cout << "Enter city C (must visit):\n> "; string C; getline(cin, C); C = trim(C);
    cout << "Enter city D (destination):\n> "; string D; getline(cin, D); D = trim(D);
    if (idx.find(A)==idx.end() || idx.find(B)==idx.end() || idx.find(C)==idx.end() || idx.find(D)==idx.end()) {
        cout << "One or more cities not found.\n"; return;
    }
    int a = idx[A], b = idx[B], c = idx[C], d = idx[D];
    bool success = false;
    vector<int> best; int bestConn = INT_MAX;
    // order A->B->C->D
    auto p1 = bfs_path(a,b);
    auto p2 = bfs_path(b,c);
    auto p3 = bfs_path(c,d);
    if (!p1.empty() && !p2.empty() && !p3.empty()) {
        auto t = join_paths(join_paths(p1,p2), p3);
        if (!t.empty()) { int conn = (int)t.size()-1; if (conn < bestConn) { bestConn = conn; best = t; success = true; } }
    }
    // order A->C->B->D
    p1 = bfs_path(a,c);
    p2 = bfs_path(c,b);
    p3 = bfs_path(b,d);
    if (!p1.empty() && !p2.empty() && !p3.empty()) {
        auto t = join_paths(join_paths(p1,p2), p3);
        if (!t.empty()) { int conn = (int)t.size()-1; if (conn < bestConn) { bestConn = conn; best = t; success = true; } }
    }
    if (!success) { cout << "No route that goes from " << A << " to " << D << " through " << B << " and " << C << "\n"; return; }
    print_path_indices(best);
    cout << "smallest number of connection: " << bestConn << "\n";
}

// DFS based tour for question 3
void dfs_tour(int u, vector<int>& visited, vector<int>& tour) {
    visited[u] = 1;
    tour.push_back(u);
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs_tour(v, visited, tour);
            // return to u in the tour after exploring v subtree
            tour.push_back(u);
        }
    }
}

void q3() {
    cout << "Enter start city A:\n> "; string A; getline(cin, A); A = trim(A);
    if (idx.find(A) == idx.end()) { cout << "City not found.\n"; return; }
    int a = idx[A];
    int n = (int)names.size();
    vector<int> visited(n,0), tour;
    dfs_tour(a, visited, tour);
    if (tour.empty()) { cout << "No reachable cities from " << A << "\n"; return; }
    print_path_indices(tour);
    cout << "smallest number of connection: " << ((int)tour.size() - 1) << "\n";
    cout << "\nNote: This route is a simple DFS-based heuristic and may not be optimal.\n";
}

// Helper BFS full distances and parents from a source
void bfs_full(int src, vector<int>& dist, vector<int>& parent) {
    int n = (int)names.size();
    dist.assign(n, -1); parent.assign(n, -1);
    queue<int> q;
    q.push(src); dist[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }
}

// reconstruct path using parent
vector<int> reconstruct(int src, int dst, const vector<int>& parent) {
    if (dst < 0) return {};
    vector<int> path;
    for (int cur = dst; cur != -1; cur = parent[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    if (!path.empty() && path.front() == src) return path;
    return {};
}

// Question 4: meeting point minimizing total connections
void q4() {
    cout << "Enter city A:\n> "; string A; getline(cin,A); A = trim(A);
    cout << "Enter city B:\n> "; string B; getline(cin,B); B = trim(B);
    cout << "Enter city C:\n> "; string C; getline(cin,C); C = trim(C);
    if (idx.find(A)==idx.end() || idx.find(B)==idx.end() || idx.find(C)==idx.end()) { cout << "One or more cities not found.\n"; return; }
    int a = idx[A], b = idx[B], c = idx[C];
    int n = (int)names.size();
    vector<int> distA, parentA, distB, parentB, distC, parentC;
    bfs_full(a, distA, parentA);
    bfs_full(b, distB, parentB);
    bfs_full(c, distC, parentC);
    int best = -1; int bestSum = INT_MAX;
    for (int cand = 0; cand < n; ++cand) {
        if (cand==a || cand==b || cand==c) continue;
        if (distA[cand] == -1 || distB[cand] == -1 || distC[cand] == -1) continue;
        int sum = distA[cand] + distB[cand] + distC[cand];
        if (sum < bestSum) { bestSum = sum; best = cand; }
    }
    if (best == -1) { cout << "No meeting city reachable by all three.\n"; return; }
    cout << "You three should meet at " << names[best] << "\n";
    auto pA = reconstruct(a, best, parentA); print_path_indices(pA); cout << " (" << distA[best] << " connections)\n";
    auto pB = reconstruct(b, best, parentB); print_path_indices(pB); cout << " (" << distB[best] << " connections)\n";
    auto pC = reconstruct(c, best, parentC); print_path_indices(pC); cout << " (" << distC[best] << " connections)\n";
    cout << "Total number of connection: " << bestSum << "\n";
}

// This is the MAIN PROGRAM LOOP
int main() {
    cout << "Route Search (STL) - Menu-driven\n";
    if (!parse_file("flight.txt")) return 1;
    cout << "Loaded " << names.size() << " cities.\n\n";
    while (true) {
        cout << "Menu:\n";
        cout << " 1) Can I fly from A to B with <= x connections? (shortest route)\n";
        cout << " 2) Route from A to D through B and C (order not important)\n";
        cout << " 3) Start at A, visit all reachable cities and return to A (simple DFS tour)\n";
        cout << " 4) Find meeting city for A, B, C minimizing total connections\n";
        cout << " 0) Exit\n";
        cout << "Choose an option: ";
        string choice; getline(cin, choice);
        if (choice == "0") { cout << "Goodbye.\n"; break; }
        else if (choice == "1") q1();
        else if (choice == "2") q2();
        else if (choice == "3") q3();
        else if (choice == "4") q4();
        else cout << "Invalid choice. Try again.\n";
        cout << "\n---\n\n";
    }
    return 0;
}
