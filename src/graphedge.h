/******************************************************************************
*
*  Udacity Nanodegree in C++
*  Memory Management ChatBot - Project 3 
*
*  Work done by Nicol Carstens, February/March 2020
*  Baseline code provided by udacity.com
*
*  Status: ready to submit (7 March 2020)
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include <vector>
#include <string>
#include <memory>

class GraphNode; // forward declaration

class GraphEdge
{
private:
    // data handles (not owned)
    GraphNode *_childNode;
    GraphNode *_parentNode;

    // proprietary members
    int _id;
    std::vector<std::string> _keywords; // list of topics associated with this edge

public:
    // constructor / desctructor
    GraphEdge(int id);

    // getter / setter
    int GetID() { return _id; }
    void SetChildNode(GraphNode *childNode);
    void SetParentNode(GraphNode *parentNode);
    GraphNode *GetChildNode() { return _childNode; }
    std::vector<std::string> GetKeywords() { return _keywords; }

    // proprietary functions
    void AddToken(std::string token);
};

#endif /* GRAPHEDGE_H_ */