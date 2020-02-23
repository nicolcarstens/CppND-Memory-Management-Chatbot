#include "graphedge.h"
#include "graphnode.h"

#include <iostream>

GraphNode::GraphNode(int id)
{
    // DEBUG std::cout << "Construct GraphNode ID = " << id << "\n";
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE - Task 0: Fix the Bug
    ////

    // std::cout << "Delete chatbot 2nd Time in GraphNode... NO LONGER \n";
    // You didn't create it. You don't own it? You shouldn't delete it.
    // FIX - REMOVE -> delete _chatBot; 

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    // Add edge to my list of parent edges that flows to my Parent Nodes ... 
    // I ("this" Node) does NOT own this edge. No ownership transfer.
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    // Note: section not highlighted for modification, but necessary... 
    //       ... unless I missed something ... 

    // Add edge to my list of child edges that flows to my Child Nodes ... 
    // I ("this" Node) owns this edge. Ownership transfer. 
    _childEdges.push_back(std::move(edge));

    // PS: good article worth reading: 
    // https://www.internalpointers.com/post/move-smart-pointers-and-out-functions-modern-c    
}

//// STUDENT CODE - Task 5: Moving the ChatBot 
////
//   WAS ... void GraphNode::MoveChatbotHere(ChatBot *chatbot)
//   ALTERNATIVE: MoveChatbotHere(std::unique_ptr<ChatBot> pChatbot)
//                if _chatBot was a unique_ptr 
//                => _chatBot.reset(std::move(pChatbot)); => correct?
//  OPTION 1: 
void GraphNode::MoveChatbotHere(ChatBot chatbot)
//  OPTION 2 ... 
//  void GraphNode::MoveChatbotHere(std::unique_ptr<ChatBot> pChatbot) 
{
    // OPTION 1: 
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    // OPTION 1: newNode->MoveChatbotHere(std::move(_chatBot));
    newNode->MoveChatbotHere(std::move(_chatBot));

    // WAS ... _chatBot = nullptr; // invalidate pointer at source
    // Nothing to be done?! 
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    // Node does NOT own Incoming Edges (these are parent edges)
    // Node DOES own Outgoing Edges (node is the parent of these child edges) 
    // We are NOT transferring ownership thus it is okay to pass a raw pointer 
    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}