#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:
    Graph();
    Graph(std::vector<std::vector<int>> matrix);
    
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    std::vector<int> neighborsOf (int nodeInd) const;
    int getGraphSize() const;
    void RemoveEdges(int nodeToRemove);
    const std::vector<std::vector<int>>& getEdges() const;

private:
    std::vector<std::vector<int>> edges;
    // infected vertices:
    std::vector<bool> vertices;
};

#endif