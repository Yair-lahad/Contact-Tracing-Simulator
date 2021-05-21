#include "../include/Tree.h"
#include "../include/Session.h"
#include <vector>
#include <queue>

using namespace std;

Tree::Tree(int rootLabel): node(rootLabel), children(){
}
// Destructor
Tree::~Tree(){ clear();}

void Tree::clear(){
    if (! children.empty()){
        for (Tree* tmpTree: children){
            delete tmpTree;
            tmpTree = nullptr;
        }
        children.clear();
    }
}
// Copy Constructor
Tree::Tree(const Tree &otherT):node(otherT.node),children(){
   copy(otherT.node,otherT.children);
}

void Tree::copy(const int &otherNode, const vector<Tree*>& otherChildren){
    node = otherNode;
    if (!otherChildren.empty()){
        int childrenSize = otherChildren.size();
        for (int i=0 ; i<childrenSize; i =i+1){
            Tree* tmpTree = otherChildren[i];
            children.push_back(tmpTree->clone());
        }

    }     
}

// Move Constructor
Tree::Tree(Tree &&otherT):node(otherT.node),children(otherT.children){
    otherT.children.clear();
}

// Copy Assignment operator
Tree& Tree::operator=(const Tree &otherT){
    if (this != &otherT){
        clear();
        copy(otherT.node, otherT.children);
    }
    return *this;
}

// Move Assignment Operator
Tree& Tree::operator=(Tree &&otherT){
    if (this != &otherT){
        clear();
        node = otherT.node;
        children = otherT.children;
        otherT.children.clear();
    }
    return *this;
}

void Tree::addChild(const Tree& child){
    Tree* childTmp = child.clone();
    children.push_back(childTmp);
}

// creates each tree by its type
Tree* Tree::createTree(const Session& session, int rootLabel){
    TreeType treeType = session.getTreeType();
    Tree* tree;
    if (treeType == Cycle)
        tree = new CycleTree(rootLabel,session.getCycle());
    else if (treeType == MaxRank)
        tree = new MaxRankTree(rootLabel);
    else 
        tree = new RootTree(rootLabel);
    return tree;
} 

int Tree::getRoot() const{
    return node;
}

Tree* Tree::getChild(int i){
    int childSize = children.size();
    if (childSize <= i)
        return nullptr;
    return children[i];
}

int Tree::getChildrenSize() const{
    if (children.empty())
        return 0;
    return children.size();
}

vector<Tree*> Tree::getChildren() const{
    return children;
}


CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel), currCycle(currCycle) {
}

int CycleTree::traceTree(){
    int i = 0;
    Tree* current = this;
    bool hasChildren = (getChildrenSize() > 0);
    // runs till the c'th left most child if exist
    while ((i < currCycle ) & hasChildren){
        current = current->getChild(0);
        i = i+1;
        hasChildren = (current->getChildrenSize()>0);
    }
    return current->getRoot();    // return the c`th left child
}

CycleTree* CycleTree::clone() const{
    return new CycleTree(*this);
}

MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){
}

int MaxRankTree::traceTree() {
    queue<Tree*> bfs;
    // holds a pointer to the Tree with the max children
    // (left most, lowest depth)
    Tree* result = this;
    Tree* currT;
    bfs.push(this);
    // holds the number of the max children size from scanned nodes
    int currMax = this->getChildrenSize();
    while(! bfs.empty()) {
         currT = bfs.front();
         bfs.pop();
         int childSize = currT->getChildrenSize();
         vector<Tree*> currChild = currT->getChildren();
         if (childSize > currMax) {
            //updates the max
            currMax = childSize;
            result = currT;
         }
         for (int i=0; i<childSize ; i=i+1) {
            bfs.push(currChild[i]);
         }
    }
    return result->getRoot();
}


MaxRankTree* MaxRankTree::clone() const{
    return new MaxRankTree(*this);
}


RootTree::RootTree(int rootLabel): Tree(rootLabel){
}

int RootTree::traceTree(){
    return this->getRoot();

}

RootTree* RootTree::clone() const{
    return new RootTree(*this);
}





