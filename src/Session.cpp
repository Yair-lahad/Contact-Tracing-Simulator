#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/Session.h"
#include "../include/Agent.h"
#include <queue>

using json = nlohmann::json;
using namespace std;

Session::Session(const std::string& path) : g(),treeType(),agents(),infectedNodes(),isAgent(),c(0) {
    // get info from json
    std::ifstream i(path);
    json j;
    i >> j;
    // update the graph with the json info
    Graph gTmp = Graph(j["graph"]);
    g = gTmp;
    string treeTypeTmp = (j["tree"]);
    if (treeTypeTmp == "C")
        treeType = Cycle;
    else if (treeTypeTmp == "M")
        treeType = MaxRank;
    else 
        treeType = Root;
    // vector indicates if nodes are agents : 
    vector<bool> isAgentTmp(j["graph"].size(),false);
    isAgent = isAgentTmp;
    for (auto& elem: j["agents"]){   
        if ( elem[0] == "C"){       
            ContactTracer CTtmp; 
            addAgent(CTtmp);
        }
        else {
            Virus Vtmp(elem[1]); 
            addAgent(Vtmp);
            // update the agents indicator:
            isAgent[elem[1]] = true;
        }
    }
}

//Destructor
Session::~Session(){clear();} 

void Session::clear(){
    if (! agents.empty()){
        for (Agent* tmpAgent: agents){
            delete tmpAgent;
            tmpAgent = nullptr;
        }
        agents.clear();
    }
}
// Copy Constructor
    Session::Session(const Session& otherS):g(otherS.g),treeType(otherS.treeType),agents(),infectedNodes(otherS.infectedNodes),isAgent(otherS.isAgent),c(otherS.c){
        copy(otherS.agents);
    }
    void Session::copy(const vector<Agent*>& otherA){               
        for (Agent* tmpAgent: otherA){
            agents.push_back(tmpAgent->clone());
        }     
    }

// Move Constructor
Session::Session(Session &&otherS):g(otherS.g),treeType(otherS.treeType),agents(otherS.agents),infectedNodes(otherS.infectedNodes),isAgent(otherS.isAgent),c(otherS.c){
    otherS.clear();
}

// Copy Assignment operator
Session& Session::operator=(const Session &otherS){
    if (this != &otherS){
        clear();
        copy(otherS.agents);
    }
    return *this;
}

// Move Assignment Operator
Session& Session::operator=(Session &&otherS){
    if (this != &otherS){
        clear();
        g = otherS.g;
        treeType = otherS.treeType;
        agents = otherS.agents;
        infectedNodes = otherS.infectedNodes;         
        isAgent = otherS.isAgent;
    }
    otherS.clear();
    return *this;
}

void Session::simulate(){
    bool finished = false;
    while (!finished) {
        // update size each iteration
        int size = agents.size();
        // act on all agents from current iteration
        for (int i=0 ; i<size ; i=i+1){
            (*agents[i]).act(*this);
        }
        // update the current cycle
        c = c+1;
        // function checks if the termination condition is met
        finished = checkSeparation();
    }

    // creating json output file
    json j;
    j["graph"] = g.getEdges();
    int agentSize = isAgent.size();
    for (int i = 0; i<agentSize ; i++) {
        if (isAgent[i])
            j["infected"].push_back(i);
    }
    cout <<"graph:" << endl;
    cout << j["graph"] << endl;
    cout <<"infected nodes:" << endl;
    cout << j["infected"] << endl;
    ofstream output("output.json");
    output << j;
}

void Session::addAgent(const Agent& agent){
    Agent* agentTmp = agent.clone();
    agents.push_back(agentTmp);
}

void Session::setGraph(const Graph& graph){
    g = graph;
}

Graph Session::getGraph() const{
    return g;          
}


int Session::getCycle() const{
    return c;
}


bool Session::infectedIsEmpty() const{
    return infectedNodes.empty();
}

bool Session::checkIsAgent(int nodeInd){
    return isAgent[nodeInd];
}

void Session::updateIsAgent(int nodeInd){
    isAgent[nodeInd] = true;
}


void Session::enqueueInfected(int a){           
    infectedNodes.push(a);
}

int Session::dequeueInfected(){
    int tmp = -1;
    if (! infectedNodes.empty()){
        tmp = infectedNodes.front();
        infectedNodes.pop();
    }
    return tmp;
}

TreeType Session::getTreeType() const{
    return treeType; 
}

// this function creates a tree by running BFS from given node
Tree* Session::BFS(int node){
    queue<Tree*> bfs;
    // True if node has been entered to queue
    vector<bool> alreadyEntered(g.getGraphSize(),false);
    Tree* wholeTree = Tree::createTree(*this, node);
    bfs.push(wholeTree);
    alreadyEntered[node] = true;
    while (! bfs.empty()){
        Tree* currentTree = bfs.front();
        bfs.pop();
        vector<int> neighbors = g.neighborsOf(currentTree->getRoot());
        int neiSize = neighbors.size();
        int childToGet = 0;
        for(int i=0; i < neiSize ; i = i+1){
            if (!(alreadyEntered[neighbors[i]])){
                Tree* currentChild = Tree::createTree(*this, neighbors[i]);
                (*currentTree).addChild(*currentChild);
                delete currentChild;
                bfs.push(currentTree->getChild(childToGet));
                alreadyEntered[neighbors[i]] = true;
                childToGet = childToGet+1;
            }
        }
    }
    return wholeTree;
}


void Session::deleteEdges(int nodeToRemove){
    g.RemoveEdges(nodeToRemove);
}

// the condition for terminating the session
bool Session::checkSeparation(){
    const vector<vector<int>> edges = g.getEdges();
    bool ans = true;
    // checks only above diagonal - undirected graph
    int edgeSize= edges.size();
    for (int i=0 ; (i<edgeSize) & ans; i=i+1){
        for (int j=i+1; (j<edgeSize) & ans; j=j+1){
            // for each edge - checks if one side is healthy...
            //               ... and one side is infected
            if (edges[i][j] == 1 )
                ans = (isAgent[i] == isAgent[j]);
        }
    }
    if (ans) {
        // checks if there is an agent not yet infected
        int edgesSize = edges.size();
        for (int i=0; (i<edgesSize) & ans; i=i+1)
            ans = isAgent[i] == g.isInfected(i);
    }
    return ans;
}