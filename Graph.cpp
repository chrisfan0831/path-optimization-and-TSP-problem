#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <algorithm>

#include <queue>
#include <climits>
#include <tuple>

using namespace std;

typedef pair<double, string> aPair;

Graph::Graph(const char* const & edgelist_csv_fn) {
    ifstream file(edgelist_csv_fn); // open the file
    if (!file.is_open()) {
        cout << "Error opening file"; 
        exit (1);
    }
    string line;                     
    string first, second, third;
    while(getline(file, line)) {  // read one line from the file
        istringstream ss(line);      // create istringstream of current line
        getline(ss, first, ',');     // store first column in "first"
        getline(ss, second, ',');    // store second column in "second"
        getline(ss, third, '\n');    // store third column column in "third"
        u.push_back(first);
        v.push_back(second);
        w.push_back(third);
    }
    file.close(); 
    //initialize array of all the nodes
    for(unsigned int i = 0; i < u.size(); i++) {
        if(find(nodeList.begin(), nodeList.end(), u[i]) == nodeList.end()) {
            nodeList.push_back(u[i]);
        }
    }
    for(unsigned int i = 0; i < v.size(); i++) {
        if(find(nodeList.begin(), nodeList.end(), v[i]) == nodeList.end()) {
            nodeList.push_back(v[i]);
        }
    }
    //initialize adjecency list
    for(auto it = nodeList.begin(); it != nodeList.end(); it++) {
        vector< pair<string, string> > adjVec;
        for(unsigned int i = 0; i < u.size(); i++) {
            if(u[i] == *it) {
                pair<string,string> vwPair = make_pair(v[i],w[i]);
                adjVec.push_back(vwPair);
            }
        }
        for(unsigned int i = 0; i < v.size(); i++) {
            if(v[i] == *it) {
                pair<string,string> uwPair = make_pair(u[i],w[i]);
                adjVec.push_back(uwPair);
            }
        }
        adjList.push_back(adjVec);
    }
}

unsigned int Graph::num_nodes() {
    unsigned int number = nodeList.size();
    return number;
}

vector<string> Graph::nodes() {
    return nodeList;
}

unsigned int Graph::num_edges() {
    unsigned int count = 0;
    unsigned int size = adjList.size(); //size of adjecency list
    for(unsigned int i = 0; i < size; i++) {
        count += adjList[i].size();
    }
    return count/2;
}

unsigned int Graph::num_neighbors(string const & node_label) {
    unsigned int count = 0;
    for(unsigned int i = 0; i < u.size(); i++) {
        if(u[i] == node_label) {
            count++;
        }
    }
    for(unsigned int i = 0; i < v.size(); i++) {
        if(v[i] == node_label) {
            count++;
        }
    }
    return count;
}

double Graph::edge_weight(string const & u_label, string const & v_label) {
    for(unsigned int i = 0; i < u.size(); i++) {
        if(u[i] == u_label) {
            if(v[i] == v_label) {
                return stod(w[i]);
            }
        }
    }
    for(unsigned int i = 0; i < v.size(); i++) {
        if(v[i] == u_label) {
            if(u[i] == v_label) {
                return stod(w[i]);
            }
        }
    }
    return -1;
}

vector<string> Graph::neighbors(string const & node_label) {
    vector<string> neighbors;
    for(unsigned int i = 0; i < nodeList.size(); i++) {
        vector<pair<string, string>> adjs = adjList[i];
        if(nodeList[i] == node_label) {
            for(unsigned int j = 0; j < adjs.size(); j++) {
                neighbors.push_back(adjs[j].first);
            }
        }
    }
    return neighbors;
}

bool Graph::BFS(string const & start_label, string const & end_label, vector<string> & pred) {
    if(start_label == end_label) {
        return true;
    }
    queue<string> q;
    vector<int> visited;
    for(unsigned int i = 0; i < nodeList.size(); i++) {
        visited.push_back(0);
    }
    auto it = find(nodeList.begin(), nodeList.end(), start_label);
    unsigned int index = it - nodeList.begin();
    visited[index] = 1;
    q.push(start_label);
    while(!q.empty()) {
        string u = q.front();
        q.pop();
        auto it = find(nodeList.begin(), nodeList.end(), u);
        unsigned int index_u = it - nodeList.begin();
        for(unsigned int i = 0; i < adjList[index_u].size(); i++) {
            string label = adjList[index_u][i].first;
            auto it = find(nodeList.begin(), nodeList.end(), label);
            unsigned int index_v = it - nodeList.begin();
            if(visited[index_v] == 0) {
                visited[index_v] = 1;
                pred[index_v] = u;
                q.push(adjList[index_u][i].first);
                if(adjList[index_u][i].first == end_label) {
                    return true;
                }
            }
        }
    }
    return false;
}

vector<string> Graph::shortest_path_unweighted(string const & end_label, string const & start_label) {
    vector<string> path;
    vector<string> pred;
    for(unsigned int i = 0; i < nodeList.size(); i++) {
        pred.push_back("");
    }
    if(BFS(start_label, end_label, pred) == false) {
        return path;
    }
    string curr = end_label;
    path.push_back(curr);
    auto iter = find(nodeList.begin(), nodeList.end(), curr);
    unsigned int index = iter - nodeList.begin();
    while(pred[index] != "") {
        path.push_back(pred[index]);
        curr = pred[index];
        auto iter = find(nodeList.begin(), nodeList.end(), curr);
        index = iter - nodeList.begin();
    }
    return path;
}

void Graph::Dijkstra(string const & start_label, vector<double> & dist, vector<string> & pred) {
    priority_queue< aPair, vector<aPair>, greater<aPair> > pq;
    vector<int> done;
    for(unsigned int i = 0; i < nodeList.size(); i++) {
        if(nodeList[i] == start_label) {
            dist.push_back(0);
        }
        else {
            dist.push_back(INT_MAX);
        }
        pred.push_back("");
        done.push_back(0);
    }
    pair<double, string> start = make_pair(0, start_label);
    pq.push(start);
    while(!pq.empty()) {
        pair<double, string> currPair = pq.top();
        pq.pop();
        string currLabel = currPair.second;
        auto it = find(nodeList.begin(), nodeList.end(), currLabel);
        unsigned int index_u = it - nodeList.begin();
        if(done[index_u] != 1) {
            done[index_u] = 1;
            for(unsigned int i = 0; i < adjList[index_u].size(); i++) {
                string label = adjList[index_u][i].first;
                double weight = stod(adjList[index_u][i].second);
                auto it = find(nodeList.begin(), nodeList.end(), label);
                unsigned int index_v = it - nodeList.begin();
                if((weight + dist[index_u]) < dist[index_v]) {
                    dist[index_v] = weight + dist[index_u];
                    pred[index_v] = nodeList[index_u];
                    pq.push(make_pair(dist[index_v], nodeList[index_v]));
                }
            }
        }
    }
}

vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & end_label, string const & start_label) {
    vector< tuple<string,string,double> > path;
    if(start_label == end_label) {
        tuple<string, string, double> edge = make_tuple(start_label, end_label, -1);
        path.push_back(edge);
        return path;
    }
    vector<double> dist;
    vector<string> pred;
    Dijkstra(start_label, dist, pred);
    auto it = find(nodeList.begin(), nodeList.end(), end_label);
    unsigned int index = it - nodeList.begin();
    if(pred[index] == "") {
        return path;
    }
    string currLabel = end_label;
    while(currLabel != start_label) {
        auto it = find(nodeList.begin(), nodeList.end(), currLabel);
        unsigned int index = it - nodeList.begin();
        string currPred = pred[index];
        double weight = 0;
        for(unsigned int i = 0; i < adjList[index].size(); i++) {
            if(adjList[index][i].first == currPred) {
                weight = stod(adjList[index][i].second);
                break;
            }
        } 
        tuple<string, string, double> edge = make_tuple(currLabel, currPred, weight);
        path.push_back(edge);
        currLabel = currPred;
    }
}


