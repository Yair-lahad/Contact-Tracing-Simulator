#ifndef AGENT_H_
#define AGENT_H_
#include "Session.h"
#include <vector>

class Agent{
public:
    Agent();
    virtual void act(Session& session)=0;
    virtual Agent* clone() const=0; 
    virtual ~Agent()=0;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session& session);
    // new function
    virtual Agent* clone() const;
};


class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual void act(Session& session);
    virtual Agent* clone() const;

private:
    const int nodeInd;
};

#endif
