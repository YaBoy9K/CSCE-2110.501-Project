#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <climits>

using namespace std;

// trim helpers
static inline string ltrim(string s){ s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch);})); return s;}
static inline string rtrim(string s){ s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch);}).base(), s.end()); return s;}
static inline string trim(string s){ return rtrim(ltrim(s)); }

vector<string> names;
unordered_map<string,int> idx;
vector<vector<int>> adj;
// This will be to return the index of a city also if it doesnt exist.
int get_index(const string &name){
    auto it = idx.find(name);
    if(it!=idx.end()) return it->second;
    int id = names.size();
    names.push_back(name);
    idx[name]=id;
    adj.emplace_back();
    return id;
}

bool parse_file(const string &filename){
    ifstream fin(filename);
    if(!fin.is_open()) return false;
    string line;
    int current=-1;
    bool expectTo=false;
    while(getline(fin,line)){
        string s=trim(line);
        if(s.empty()){ expectTo=false; continue; }
        if(s.rfind("From:",0)==0){
            string city=trim(s.substr(5));
            current=get_index(city);
            expectTo=false;
        }else if(s.rfind("To",0)==0){
            auto pos=s.find(':');
            if(pos!=string::npos){
                string rest=trim(s.substr(pos+1));
                if(!rest.empty()){
                    int id=get_index(rest);
                    if(current>=0) adj[current].push_back(id);
                }
            }
            expectTo=true;
        }else if(expectTo){
            int id=get_index(s);
            if(current>=0) adj[current].push_back(id);
        }
    }
    return true;
}

string extractCountry(const string &s){
    auto pos=s.rfind(',');
    if(pos==string::npos) return s;
    return trim(s.substr(pos+1));
}

string extractCity(const string &s){
    auto pos=s.rfind(',');
    if(pos==string::npos) return s;
    return trim(s.substr(0,pos));
}

struct CountryGroup{
    string countryName;
    vector<int> cityIndices;
    int size(){
                return cityIndices.size();
    }
};

vector<CountryGroup> countries;

void buildCountries(){
    unordered_map<string,int> mp;
    for(int i=0;i<names.size();i++){
        string c=extractCountry(names[i]);
        if(mp.find(c)==mp.end()){
            mp[c]=countries.size();
            countries.push_back({c,{}});
        }
        countries[mp[c]].cityIndices.push_back(i);
    }
}

void printCountries(){
    cout<<"List of countrys:\n";
    for(int i=0;i<countries.size();i++){
        cout<<" "<<i<<")"<<countries[i].countryName<<"\n";
    }
    cout<<"...\n";
}

void printCitiesFromCountry(int c){
    cout<<"List of citys\n";
    for(int i=0;i<countries[c].cityIndices.size();i++){
        int id=countries[c].cityIndices[i];
        cout<<i<<")"<<extractCity(names[id])<<"\n";
    }
}

vector<int> bfs_path(int src,int dst){
    int n=names.size();
    vector<int> parent(n,-1),dist(n,-1);
    queue<int>q; q.push(src); dist[src]=0;
    while(!q.empty()){
        int u=q.front(); q.pop();
        if(u==dst) break;
        for(int v:adj[u]){
            if(dist[v]==-1){
                dist[v]=dist[u]+1;
                parent[v]=u;
                q.push(v);
            }
        }
    }
    if(dist[dst]==-1) return {};
    vector<int> p;
    for(int cur=dst;cur!=-1;cur=parent[cur]) p.push_back(cur);
    reverse(p.begin(),p.end());
    return p;
}

void print_path_indices(const vector<int>& path){
    for(int i=0;i<path.size();i++){
        cout<<names[path[i]];
        if(i+1<path.size()) cout<<" to ";
    }
    cout<<"\n";
}

void q1(){
    string t;
    bool fail = false;
    printCountries();
    int fromC;
    int countryListSize = countries.size()-1;
    do {
                fail = false;
                cout<<"\nEnter valid FROM Country number: ";
                getline(cin,t);
                try
                {
                                fromC=stoi(t);
                }
                catch(std::exception)
                {
                                fail = true;
                }
                
                
    } while(fail || fromC < 0 || fromC > countryListSize);
    
    printCitiesFromCountry(fromC);
    
    int a = -1;
    do
    {
                fail = false;
                cout<<"\nEnter valid FROM City number: ";
                getline(cin,t);
                try{a = stoi(t);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || a > countries[fromC].size()-1 || a < 0);
    
    int fromCity=countries[fromC].cityIndices[a];

    printCountries();
    int toC;
    do{          
                fail = false;
                cout<<"\nEnter valid TO Country number: ";
                getline(cin,t);
                try{toC=stoi(t);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || toC > countryListSize || toC < 0);
    
    

    printCitiesFromCountry(toC);
    do
    {
                fail = false;
                cout<<"\nEnter valid TO City number: ";
                getline(cin,t);
                try{ a = stoi(t);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || a < 0 || a > countries[toC].size()-1);
    
    int toCity=countries[toC].cityIndices[a];

    auto path=bfs_path(fromCity,toCity);
    if(path.empty()){ cout<<"No route found.\n"; return; }

    print_path_indices(path);
    cout<<"total connection: "<<(path.size()-1)<<"\n";
}

// Helper to join two paths where p1 end == p2 start
vector<int> join_paths(const vector<int>& p1, const vector<int>& p2) {
    if (p1.empty() || p2.empty()) return {}; // if either are empty
    if (p1.back() != p2.front()) return {}; // if the two paths dont meet
    vector<int> out = p1;
    out.insert(out.end(), p2.begin()+1, p2.end()); // add p2 to p1
    return out;
}

// Question 2: A -> D through B and C
void q2() {
    bool fail;
    int tmpA;
    int countryListSize = countries.size()-1;
    string A;

    printCountries();

    do{
                fail = false;
                cout << "\nEnter valid starting Country: "; 
                getline(cin, A);
                try{tmpA = stoi(A);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpA > countryListSize || tmpA < 0);
    printCitiesFromCountry(tmpA);
    int cA;
    string cityA;
    do
    {
                fail = false;
                cout << "\nEnter valid starting City: ";
                getline(cin, cityA);
                try{cA = stoi(cityA);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cA > countries[tmpA].size()-1 || cA < 0);
    
    printCountries();
    int tmpB;
    string B;

    do{
                fail = false;
                cout << "Enter valid second Country:\n> "; 
                getline(cin, B); 
                try{tmpB = stoi(B);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpB > countryListSize || tmpB < 0);
    printCitiesFromCountry(tmpB);
    int cB;
    string cityB;
    do
    {
                fail = false;
                cout << "\nEnter valid second City: ";
                getline(cin, cityB);
                try{cB = stoi(cityB);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cB > countries[tmpB].size()-1 || cB < 0);
    
    printCountries();
    int tmpC;
    string C;
    do{
                fail = false;
                cout << "Enter valid third Country:\n> "; 
                getline(cin, C); 
                try{tmpC = stoi(C);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpC > countryListSize || tmpC < 0);

    printCitiesFromCountry(tmpC);
    int cC;
    string cityC;
    do
    {
                fail = false;
                cout << "\nEnter valid third City: ";
                getline(cin, cityC);
                try{cC = stoi(cityC);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while(fail || cC > countries[tmpA].size()-1 || cC < 0);

    printCountries();
    int tmpD;
    string D;
    do{
                fail = false;
                cout << "Enter valid last Country:\n> "; 
                getline(cin, D); 
                try{tmpD = stoi(D);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpD > countryListSize || tmpD < 0);

    printCitiesFromCountry(tmpD);
    int cD;
    string cityD;
    do
    {
                fail = false;
                cout << "\nEnter valid destination City: ";
                getline(cin, cityD);
                try{cD = stoi(cityD);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cD > countries[tmpD].size()-1 || cD < 0);


    //int a = idx[A], b = idx[B], c = idx[C], d = idx[D]; //set indexes

    bool success = false;
    vector<int> best; int bestConn = INT_MAX;

    // order A->B->C->D
    auto p1 = bfs_path(cA,cB);
    auto p2 = bfs_path(cB,cC);
    auto p3 = bfs_path(cC,cD);

    if (!p1.empty() && !p2.empty() && !p3.empty()) { //if all paths contain nodes
        auto t = join_paths(join_paths(p1,p2), p3);  //join the paths together
        if (!t.empty()) { 
                int conn = (int)t.size()-1; //connection is highest index of t
                                if (conn < bestConn) { 
                                                bestConn = conn; best = t; success = true; //if the connection is shorter than INT_MAX
                                } 
                }
    }


    //repeat process
    // order A->C->B->D
    p1 = bfs_path(cA,cC);
    p2 = bfs_path(cC,cB);
    p3 = bfs_path(cB,cD);

    if (!p1.empty() && !p2.empty() && !p3.empty()) {
        auto t = join_paths(join_paths(p1,p2), p3);
        if (!t.empty()) { int conn = (int)t.size()-1; if (conn < bestConn) { bestConn = conn; best = t; success = true; } }
    }

    //if all attempts fail
    if (!success) { cout << "No route that goes from " << A << " to " << D << " through " << B << " and " << C << "\n"; return; }

    print_path_indices(best); //print path
    cout << "smallest number of connection: " << bestConn << "\n";
}

// DFS based tour for question 3
void dfs_tour(int u, vector<int>& visited, vector<int>& tour) {
    visited[u] = 1; //first node is visited
    tour.push_back(u); //first node loaded in the tour
    for (int v : adj[u]) { //grab the index of current
        if (!visited[v]) {
            dfs_tour(v, visited, tour); // recursive call
            // return to u in the tour after exploring v subtree
            tour.push_back(u);
        }
    }
}

void q3() {
    string A;
    int tmpA;
    bool fail;
    int countryListSize = countries.size()-1;

    printCountries();
    do{
                fail = false;
                cout << "\nEnter start Country: "; 
                getline(cin, A); 
                try{tmpA = stoi(A);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpA > countryListSize || tmpA < 0);

    printCitiesFromCountry(tmpA);
    int cA;
    string cityA;
    do{
                fail = false;
                cout << "\nEnter valid starting City: ";
                getline(cin, cityA);
                try{cA = stoi(cityA);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cA > countries[tmpA].size()-1 || cA < 0);

    int n = (int)names.size();

    vector<int> visited(n,0), tour; //visited is names.length long and is full of 0,   makes tour
    dfs_tour(cA, visited, tour); 

    if (tour.empty()) { cout << "No reachable cities from " << A << "\n"; return; }

    print_path_indices(tour);
    cout << "smallest number of connection: " << ((int)tour.size() - 1) << "\n";
    cout << "\nNote: This route is a simple DFS-based heuristic and may not be optimal.\n";
}

/**
 * Helper BFS full distances and parents from a source
 * takes in the references of distance and parent
 */
void bfs_full(int src, vector<int>& dist, vector<int>& parent) {
    int n = (int)names.size(); // size of names vector
    
    //reinitializes dist and parent size of n and full of -1
    dist.assign(n, -1); 
    parent.assign(n, -1);

    //BFS loop
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
    bool fail;
    int tmpA;
    int countryListSize = countries.size()-1;
    string A;

    printCountries();

    do{
                fail = false;
                cout << "\nEnter valid starting Country: "; 
                getline(cin, A);
                try{tmpA = stoi(A);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpA > countryListSize || tmpA < 0);
    printCitiesFromCountry(tmpA);
    int cA;
    string cityA;
    do
    {
                fail = false;
                cout << "\nEnter valid starting City: ";
                getline(cin, cityA);
                try{cA = stoi(cityA);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cA > countries[tmpA].size()-1 || cA < 0);
    
    printCountries();
    int tmpB;
    string B;

    do{
                fail = false;
                cout << "\nEnter valid second Country: "; 
                getline(cin, B); 
                try{tmpB = stoi(B);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpB > countryListSize || tmpB < 0);
    printCitiesFromCountry(tmpB);
    int cB;
    string cityB;
    do{
                fail = false;
                cout << "\nEnter valid second City: ";
                getline(cin, cityB);
                try{cB = stoi(cityB);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while (fail || cB > countries[tmpB].size()-1 || cB < 0);
    
    printCountries();
    int tmpC;
    string C;
    do{
                fail = false;
                cout << "\nEnter valid third Country: "; 
                getline(cin, C); 
                try{tmpC = stoi(C);}
                catch(std::exception)
                {
                                fail = true;
                }
    }while(fail || tmpC > countryListSize || tmpC < 0);

    printCitiesFromCountry(tmpC);
    int cC;
    string cityC;
    do{
                fail = false;
                cout << "\nEnter valid third City: ";
                getline(cin, cityC);
                try{cC = stoi(cityC);}
                catch(std::exception)
                {
                                fail = true;
                }
    } while(fail || cC > countries[tmpA].size()-1 || cC < 0);
    int n = (int)names.size();
    vector<int> distA, parentA, distB, parentB, distC, parentC;

    bfs_full(cA, distA, parentA);
    bfs_full(cB, distB, parentB);
    bfs_full(cC, distC, parentC);

    int best = -1; int bestSum = INT_MAX;
    for (int cand = 0; cand < n; ++cand) {
        if (cand==cA || cand==cB || cand==cC) continue;
        if (distA[cand] == -1 || distB[cand] == -1 || distC[cand] == -1) continue;
        int sum = distA[cand] + distB[cand] + distC[cand];
        if (sum < bestSum) { bestSum = sum; best = cand; }
    }

    if (best == -1) { cout << "No meeting city reachable by all three.\n"; return; }

    cout << "You three should meet at " << names[best] << "\n";
    auto pA = reconstruct(cA, best, parentA); print_path_indices(pA); cout << " (" << distA[best] << " connections)\n";
    auto pB = reconstruct(cB, best, parentB); print_path_indices(pB); cout << " (" << distB[best] << " connections)\n";
    auto pC = reconstruct(cC, best, parentC); print_path_indices(pC); cout << " (" << distC[best] << " connections)\n";
    cout << "Total number of connection: " << bestSum << "\n";
}

int main(){
    cout<<"Route Search (Updated Version)\n";
    if(!parse_file("flight.txt")){
        cout<<"Could not load flight.txt\n";
        return 1;
    }
    buildCountries();
    bool loadLoop = true;
    while(loadLoop){
        cout<<"\nMenu:\n";
        cout<<"1) Search route using country/city selection\n";
        cout<<"2) Route from A to D through B and C (order not important)\n";
        cout<<"3) Start at A, visit all reachable cities and return to A (simple DFS tour)\n";
        cout << "4) Find meeting city for A, B, C minimizing total connections\n";
        cout<<"0) Exit\n";
        cout<<"Choose: ";
        string c; getline(cin,c);
        int choice;
        try{
                choice = stoi(c);
        }catch(exception){
                choice = -1;
        }
        
        switch(choice){
                case 0 : cout << "Goodbye.\n"; loadLoop = false; break;
                case 1: q1(); break;
                case 2: q2(); break;
                case 3: q3(); break;
                case 4: q4(); break;
                default: cout << "Invalid choice. Try again.\n";
        }

    }
    return 0;
}
