#ifndef TREE_H_
#define TREE_H_

#include <vector>

class Session;

class Tree{
public:

    Tree(int rootLabel);
    // destructor
    virtual ~Tree();
    void clear();

    // Copy Constructor
    Tree(const Tree &otherT);
    void copy(const int &otherNode, const std::vector<Tree*>& otherChildren);

    // Move Constructor
    Tree(Tree &&otherT);
    
    // Copy Assignment Operator
    Tree& operator=(const Tree &otherT);

    // Move Assignment Operator
    Tree& operator=(Tree &&otherT);

    void addChild(const Tree& child);
    virtual Tree* clone() const=0;

    //Getters
    int getRoot() const;
    Tree* getChild(int i);
    int getChildrenSize() const;
    std::vector<Tree*> getChildren() const;

    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

private:
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual CycleTree* clone() const;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    virtual MaxRankTree* clone() const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual RootTree* clone() const;
};

#endif