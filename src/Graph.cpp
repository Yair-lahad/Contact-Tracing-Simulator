#include "../include/Graph.h"
#include <vector>

using namespace std;

Graph::Graph():edges(),vertices(){
}

Graph::Graph(std::vector<std::vector<int>> matrix): edges(matrix),vertices(){
    vector<bool> verticesTmp(matrix.size(),false);
    vertices = verticesTmp;
}
void Graph::infectNode(int nodeInd){
    // True if entered the infected node queue
    vertices[nodeInd] = true;
}

bool Graph::isInfected(int nodeInd){
    return vertices[nodeInd];
}

//function returns all the neighbors of a specific node
vector<int> Graph::neighborsOf(int nodeInd) const{
    vector<int> neighbors;
    int edgeSize = edges.size();
    for (int i=0 ; i<edgeSize ; i =i+1){
        if (edges[nodeInd][i] == 1)
            neighbors.push_back(i);
    }
    return neighbors;
}

int Graph::getGraphSize() const{
    return edges.size();
}    

void Graph::RemoveEdges(int nodeToRemove){
    vector<int> nei = neighborsOf(nodeToRemove);
    int edgeSize = edges.size();
    // remove my edges:
    for (int i=0; i<edgeSize; i=i+1){
        edges[nodeToRemove][i] = 0;
    }
    // remove from neighbors:
    for (int i: nei)
        edges[i][nodeToRemove] = 0;   
}

const vector<vector<int>>& Graph::getEdges() const{
    return edges;
}


