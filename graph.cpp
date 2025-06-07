#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <iostream>
#include "graph.h"

using namespace std;

void graph::addNode(const string& node) {
    if (adjList.find(node) == adjList.end()) {
        adjList[node] = vector<string>();
    }
}

void graph::addEdge(const string& u, const string& v) {
    addNode(u);
    addNode(v);#ifndef GRAPH_CPP
    #define GRAPH_CPP
    
    #include <iostream>
    #include "graph.h"
    
    using namespace std;
    
    void graph::addNode(const string& node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = vector<string>();
        }
    }
    
    void graph::addEdge(const string& u, const string& v) {
        addNode(u);
        addNode(v);
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    
    void graph::printGraph() {
        for (const auto& pair : adjList) {
            cout << pair.first << ": ";
            for (const string& neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
    
    bool graph::hasNode(const string& node) {
        return adjList.find(node) != adjList.end();
    }
    
    bool graph::hasEdge(const string& u, const string& v) {
        if (!hasNode(u)) {
            return false;
        } 
        for (const string& neighbor : adjList[u]) {
            if (neighbor == v) {
                return true;
            }
        }
        return false;
    }
    #endif
    adjList[u].push_back(v);
    adjList[v].push_back(u);
}

void graph::printGraph() {
    for (const auto& pair : adjList) {
        cout << pair.first << ": ";
        for (const string& neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

bool graph::hasNode(const string& node) {
    return adjList.find(node) != adjList.end();
}

bool graph::hasEdge(const string& u, const string& v) {
    if (!hasNode(u)) {
        return false;
    } 
    for (const string& neighbor : adjList[u]) {
        if (neighbor == v) {
            return true;
        }
    }
    return false;
}
#endif