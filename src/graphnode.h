#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
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
    // Alternative:
    // -->> [ChatBot *_chatBot] => [std::unique_ptr<ChatBot> _chatBot] ? 
    ChatBot _chatBot;

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
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    //// STUDENT CODE - Task 5 : Moving the ChatBot
    ////

    //  WAS ... void MoveChatbotHere(ChatBot *chatbot);
    //  Then I made it ... void MoveChatbotHere(ChatBot &&chatbot);
    //  But then Move Constructor is not called ...  
    void MoveChatbotHere(ChatBot chatbot);    

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */