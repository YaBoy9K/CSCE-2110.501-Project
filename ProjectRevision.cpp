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

    printCountries();
    cout<<"\nEnter FROM Country number: ";
    getline(cin,t);
    int fromC=stoi(t);

    printCitiesFromCountry(fromC);
    cout<<"\nEnter From City number: ";
    getline(cin,t);
    int fromCity=countries[fromC].cityIndices[stoi(t)];

    printCountries();
    cout<<"\nEnter TO Country number: ";
    getline(cin,t);
    int toC=stoi(t);

    printCitiesFromCountry(toC);
    cout<<"\nEnter From City number: ";
    getline(cin,t);
    int toCity=countries[toC].cityIndices[stoi(t)];

    auto path=bfs_path(fromCity,toCity);
    if(path.empty()){ cout<<"No route found.\n"; return; }

    print_path_indices(path);
    cout<<"total connection: "<<(path.size()-1)<<"\n";
}

int main(){
    cout<<"Route Search (Updated Version)\n";
    if(!parse_file("flight.txt")){
        cout<<"Could not load flight.txt\n";
        return 1;
    }
    buildCountries();

    while(true){
        cout<<"\nMenu:\n";
        cout<<"1) Search route using country/city selection\n";
        cout<<"0) Exit\n";
        cout<<"Choose: ";
        string c; getline(cin,c);
        if(c=="0") break;
        if(c=="1") q1();
        else cout<<"Invalid\n";
    }
    return 0;
}
