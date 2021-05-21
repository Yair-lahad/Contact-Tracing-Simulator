#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <queue>
#include "Tree.h"

class Agent;



enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);
    // destructor
    virtual ~Session();
    void clear();

    // Copy Constructor
    Session(const Session& otherS);
    void copy(const std::vector<Agent*>& otherA);
    
    // Move Constructor
    Session(Session &&otherS);

    // Copy Assignment Operator
    Session& operator=(const Session& otherS);

    // Move Assignment Operator
    Session& operator=(Session &&otherS);

    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);

    // Getters
    Graph getGraph() const;
    int getCycle() const;
    bool infectedIsEmpty() const;
    bool checkIsAgent(int nodeInd);
    void updateIsAgent(int nodeInd);
    bool checkSeparation();
    
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    Tree* BFS(int node);
    void deleteEdges(int nodeToremove);
    

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    // new fields :
    std::queue<int> infectedNodes;
    // indicates if a node is already agent
    std::vector<bool> isAgent;
    int c;

};

#endif
