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

//===================================================================
//   WAS ... void GraphNode::MoveChatbotHere(ChatBot *chatbot)
//   NOW ....void GraphNode::MoveChatbotHere(ChatBot chatbot)
//   ALTERNATIVE: MoveChatbotHere(std::unique_ptr<ChatBot> pChatbot)
//                if _chatBot was a unique_ptr 
//                ... but ... NOT AN OPTION?! 
//  Requirement calls for an instance (not ptr) being moved here  
//===================================================================
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));

    // WAS ... _chatBot = nullptr; // invalidate pointer at source
    // Nothing to be done? No? Smart Pointer takes care of source. 
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