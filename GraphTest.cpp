#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include "Graph.h"
using namespace std;

// define test names
#ifndef PROPERTIES
#define PROPERTIES "graph_properties"
#endif
#ifndef BFS
#define BFS "shortest_unweighted"
#endif
#ifndef DIJKSTRA
#define DIJKSTRA "shortest_weighted"
#endif
#ifndef COMPONENTS
#define COMPONENTS "connected_components"
#endif
#ifndef THRESHOLD
#define THRESHOLD "smallest_threshold"
#endif

// valid tests
const string TESTS_ORDERED[] = {PROPERTIES, BFS, DIJKSTRA, COMPONENTS, THRESHOLD};
const unordered_set<string> TESTS(TESTS_ORDERED, TESTS_ORDERED+sizeof(TESTS_ORDERED)/sizeof(string));

// main GraphTest program execution
int main(int argc, char** argv) {
    // check user args
    if(argc != 3) {
        cerr << "USAGE: " << argv[0] << " <edgelist_csv> <test>" << endl; exit(1);
    } else if(TESTS.find(argv[2]) == TESTS.end()) {
        cerr << "Invalid test: " << argv[2] << endl;
        cerr << "Valid options:" << endl;
        for(const string & test : TESTS_ORDERED) {
            cerr << "* " << test << endl;
        }
        exit(1);
    }

    // load graph and perform tests
    Graph g(argv[1]);
    vector<string> nodes = g.nodes();

    // check basic properties of the graph
    if(strcmp(argv[2], PROPERTIES) == 0) {
        cout << "Number of Nodes\t" << g.num_nodes() << endl;
        cout << "Nodes\t"; bool first = true;
        for(string const & u : nodes) {
            if(first) { cout << '{'; first = false; } else { cout << ','; }
            cout << '"' << u << '"';
        }
        if(first) { cout << '{'; }
        cout << '}' << endl;
        cout << "Number of Edges\t" << g.num_edges() << endl;
        cout << "Edge Weights\t"; first = true;
        for(string const & u : nodes) {
            bool first_in = true;
            if(first) { cout << '{'; first = false; } else { cout << ','; }
            cout << '"' << u << "\":";
            for(string const & v : nodes) {
                if(first_in) { cout << '{'; first_in = false; } else { cout << ','; }
                cout << '"' << v << "\":" << g.edge_weight(u,v);
            }
            if(first_in) { cout << '{'; }
            cout << '}';
        }
        if(first) { cout << '{'; }
        cout << '}' << endl;
        cout << "Number of Neighbors\t"; first = true;
        for(string const & u : nodes) {
            if(first) { cout << '{'; first = false; } else { cout << ','; }
            cout << '"' << u << "\":" << g.num_neighbors(u);
        }
        if(first) { cout << '{'; }
        cout << '}' << endl;
        cout << "Neighbors\t"; first = true;
        for(string const & u : nodes) {
            bool first_in = true;
            if(first) { cout << '{'; first = false; } else { cout << ','; }
            cout << '"' << u << "\":";
            for(string const & v : g.neighbors(u)) {
                if(first_in) { cout << '{'; first_in = false; } else { cout << ','; }
                cout << '"' << v << '"';
            }
            if(first_in) { cout << '{'; }
            cout << '}';
        }
        if(first) { cout << '{'; }
        cout << '}' << endl;
    }

    // check shortest path
    else if(strcmp(argv[2], BFS) == 0 || strcmp(argv[2], DIJKSTRA) == 0) {
        bool is_bfs = strcmp(argv[2], DIJKSTRA); bool first = true;
        cout << "Shortest Path ("; if(is_bfs) { cout << "Unw"; } else { cout << "W"; } cout << "eighted)\t";
        for(string const & u : nodes) {
            bool first_in = true;
            if(first) { cout << '{'; first = false; } else { cout << ','; }
            cout << '"' << u << "\":";
            for(string const & v : nodes) {
                bool first_in_in = true;
                if(first_in) { cout << '{'; first_in = false; } else { cout << ','; }
                cout << '"' << v << "\":";
                if(is_bfs) {
                    for(string const & curr : g.shortest_path_unweighted(u,v)) {
                        if(first_in_in) { cout << '['; first_in_in = false; } else { cout << ','; }
                        cout << '"' << curr << '"';
                    }
                } else {
                    for(tuple<string,string,double> const & curr : g.shortest_path_weighted(u,v)) {
                        if(first_in_in) { cout << '['; first_in_in = false; } else { cout << ','; }
                        cout << "(\"" << get<0>(curr) << "\",\"" << get<1>(curr) << "\"," << get<2>(curr) << ')';
                    }
                }
                if(first_in_in) { cout << '['; }
                cout << ']';
            }
            if(first_in) { cout << '{'; }
            cout << '}';
        }
        if(first) { cout << '{'; }
        cout << '}' << endl;
    }

}
