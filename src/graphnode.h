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
******************************************************************************
*
*  TASK 4: Moving Smart Pointers
*
*  In files chatlogic.h / chatlogic.cpp and graphnodes.h / graphnodes.cpp all 
*  instances of GraphEdge are changed in a way such that each instance of 
*  GraphNode exclusively owns the outgoing GraphEdges and holds non-owning 
*  references to incoming GraphEdges. Appropriate smart pointers are used to 
*  do this. Where required, changes are made to the code such that data 
*  structures and function parameters reflect the changes.
*
*  In files chatlogic.h / chatlogic.cpp and graphnodes.h / graphnodes.cpp, 
*  move semantics are used when transferring ownership from class ChatLogic, 
*  where all instances of GraphEdge are created, into instances of GraphNode.
* 
******************************************************************************/

#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include <memory>
#include "chatbot.h"

// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE - Task 4 : Moving Smart Pointers 
    ////

    // data handles (owned)
    // Node DOES own Outgoing Edges (edges to subsequent nodes)
    // Node is the parent of these child edges
    // WAS ... std::vector<GraphEdge *> _childEdges;  
    std::vector<std::unique_ptr<GraphEdge>> _childEdges; 

    // data handles (not owned), and thus leave it a "raw pointer" vector
    // NC QUESION: would it be better or worse to use weak pointer? no diffs?
    // Node does NOT own Incoming Edges (edges to preceding nodes)
    std::vector<GraphEdge *> _parentEdges; 
    
    // Node did not own the ChatBot ... now it does!
    // WAS .. ChatBot *_chatBot;
    ChatBot _chatBot;

    // OPTION 2 ... ? Not an option given requirements Task 5, but otherwise 
    //                do no see a reason why it would not be an option 
    //                If it was a pointer transferring ownership you would
    //                not see the move semantics in action 
    // std::unique_ptr<ChatBot> _chatBot;  

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);

    // Two options to pass edge -> lvalue reference or rvalue
    // See call in chatlogic.cpp -> (*parentNode)->AddEdgeToChildNode(edge);
    // https://www.internalpointers.com/post/move-smart-pointers-and-out-functions-modern-c 
    // OPTION 1: GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
    // OPTION 2: GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> &edge)
    //           pass edge by lvalue reference rather than move ... 
    //           ... but lvalue reference can't be called with rvalue!
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> &edge);

    //// STUDENT CODE - Task 5 : Moving the ChatBot
    ////

    //  WAS ... void MoveChatbotHere(ChatBot *chatbot);
    //  Then I made it ... void MoveChatbotHere(ChatBot &&chatbot);
    //  But then Move Constructor is not called ... 
    //  OPTION 1: 
    void MoveChatbotHere(ChatBot chatbot); 
    
    //  OPTION 2: ... makes ownership very clear? 
    //  ... but you won't be calling the ChatBot Move Constructor
    //  ... and call to ChatBot Move Constructor is required!! 
    //  2.1 => void MoveChatbotHere(std::unique_ptr<ChatBot> &chatbot); 
    //  2.2 => void MoveChatbotHere(std::unique_ptr<ChatBot> chatbot);  
    //  NB: 2.2 can be called with lvalue or rvalue and 2.1 only with lvalue  

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */