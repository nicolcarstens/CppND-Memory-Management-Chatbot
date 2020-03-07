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
*  TASK 3: Exclusive Ownership 2
* 
*  In file chatlogic.h / chatlogic.cpp, the vector _nodes are adapted in a 
*  way that the instances of GraphNodes to which the vector elements refer 
*  are exclusively owned by the class ChatLogic. An appropriate type of smart 
*  pointer is used to achieve this.
*
******************************************************************************/

#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
#include "chatgui.h"

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE - Task 3: Exclusive Ownership 2
    ////

    // data handles (owned)
    // WAS... std::vector<GraphNode *> _nodes;
    // WAS... std::vector<GraphEdge *> _edges;
    std::vector<std::unique_ptr<GraphNode>> _nodes;

    // NC REMOVED: std::vector<std::unique_ptr<GraphEdge>> _edges; 
    // IS THIS EVEN STILL USED? NC REMOVED

    ////
    //// EOF STUDENT CODE

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */