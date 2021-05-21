#include "../include/Agent.h"
#include <vector>

using namespace std;

Agent::Agent(){}

Agent::~Agent(){}

ContactTracer::ContactTracer(){}


// Methods:
void ContactTracer::act(Session& session){
    // only acts if there are infected vertices
    if (!session.infectedIsEmpty()){
        int nodeForBFS = session.dequeueInfected();
        // builds the BFS tree, starting from nodeForBFS
        Tree* currentTree = session.BFS(nodeForBFS);
        // checks which node to remove
        int nodeToRemove = currentTree->traceTree();
        delete(currentTree);
        // removes the node edges
        session.deleteEdges(nodeToRemove);       
    }                                                    
}

Agent* ContactTracer::clone() const{
    return new ContactTracer(*this);
}


Virus::Virus(int nodeInd):nodeInd(nodeInd){       
}

void Virus::act(Session& session){
    Graph tmpG = session.getGraph();                       
    // add to infected queue
    if (! tmpG.isInfected(nodeInd)){
        tmpG.infectNode(nodeInd);   // mark T 
        session.enqueueInfected(nodeInd);
    }
    // infect neighbor - new agent
    bool foundHealthy = false;
    int i = 0;
    vector<int> neighbors = tmpG.neighborsOf(nodeInd);
    int neiSize = neighbors.size();
    // only acts if has a healthy neighbor
    while ((!foundHealthy) & (i<neiSize)){
        if (! session.checkIsAgent(neighbors[i])){
            Virus virusTmp(neighbors[i]);
            session.addAgent(virusTmp);
            // update the indicator after added to Agents list 
            session.updateIsAgent(neighbors[i]);
            foundHealthy = true;
        }
        i=i+1;
    }
    // update the original Graph
    session.setGraph(tmpG);
}

Agent* Virus::clone() const{
    return new Virus(*this);
}


