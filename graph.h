#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string.h>
#include <vector>
#include <cctype>
#include <unordered_map>

using namespace std;

class graph {
    public:
        unordered_map<string, vector<string>> adjList;

        void addNode(const string& node);
        void addEdge(const string& u, const string& v);
        void printGraph();
        bool hasNode(const string& node);
        bool hasEdge(const string& u, const string& v);
};

#endif