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
******************************************************************************
*
*  TASK 5: Moving the ChatBot
*
*  In file chatlogic.cpp, a local ChatBot instance is created on the stack at 
*  the bottom of function LoadAnswerGraphFromFile and move semantics are used 
*  to pass the ChatBot instance into the root node.
*
*  ChatLogic has no ownership relation to the ChatBot instance and thus is no 
*  longer responsible for memory allocation and deallocation.
*
*  When the program is executed, messages are printed to the console indicating 
*  which Rule of Five component of ChatBot is being called.
* 
******************************************************************************/

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE - Task 5 : Moving the ChatBot
    ////

    // create instance of chatbot
    // ... no longer ... moved to stack at bottom of LoadAnswerGraphFromFile
    // _chatBot = new ChatBot("../images/chatbot.png");
    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    // _chatBot->SetChatLogicHandle(this);

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE - Task 5 : Moving the ChatBot
    ////

    // delete chatbot instance
    // REMOVED in Task 5 : Moving the ChatBot
    // std::cout << "Deleting chatbot 1st time ... in chatlogic...\n";
    // delete _chatBot;

    // Task 3: Exclusive Ownership 2
    // delete all nodes - UPDATE: no longer required. removed. 
    // REMOVED.. for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    // REMOVED..{
    // REMOVED..    delete *it;
    // REMOVED..}

    // delete all edges - UPDATE: no longer required. removed.
    // REMOVED..for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    // REMOVED..{
    // REMOVED..    delete *it;
    // REMOVED..}

    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE - Task 3: Exclusive Ownership 2
                        ////

                        // check if node with this ID exists already
                        // WAS ... auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](GraphNode *node) { return node->GetID() == id; });
                        // https://www.internalpointers.com/post/move-smart-pointers-and-out-functions-modern-c
                        // I DO NOT WANT TO ALTER OWNERSHIP!! BUT I CAN"T GET THIS TO COMPILE ANY OTHER WAY? 
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](const std::unique_ptr<GraphNode> &node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            // NC CHANGES... 
                            // WAS ... _nodes.emplace_back(new GraphNode(id));
                            // NOW ... create a new GraphNode and covert pointer to unique_ptr, 
                            //         transferring ownership in the process 

                            _nodes.emplace_back(std::make_unique<GraphNode>(id));

                            // Alternative would be make_unique? Same effect? 
                            // But less safe way to create a unique_ptr
                            // _nodes.emplace_back(std::unique_ptr<GraphNode>(new GraphNode(id)));

                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, *(newNode->get()));
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE - Task 3: Exclusive Ownership 2
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(parentToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); });

                            // create new edge   
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);

                            edge->SetChildNode((*childNode).get());
                            edge->SetParentNode((*parentNode).get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);
                            // AddAllTokensToElement("KEYWORD", tokens, *_edges.back().get());

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());

                            // Two options to pass edge -> lvalue reference or rvalue
                            // https://www.internalpointers.com/post/move-smart-pointers-and-out-functions-modern-c
                            // 
                            // OPTION 1: 
                            //    GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
                            //    (*parentNode)->AddEdgeToChildNode(std::move(edge));
                            //
                            // OPTION 2:
                            //    pass edge by lvalue reference rather than move ... 
                            //    GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> &edge)
                            //    ... but lvalue reference can't be called with rvalue!
                            
                            (*parentNode)->AddEdgeToChildNode(edge);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE - Task 3: Exclusive Ownership 2
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {
            if (rootNode == nullptr)
            {
                // WAS ... rootNode = *it; // assign current node to root

                // rootNode = (*it).get(); // assign current node to root
                                           // do not transfer ownership
                rootNode = it->get();
            }

            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    //=============================================================================
    // OPTION 1: 
    ChatBot chatBot("../images/chatbot.png");
    //=============================================================================
    // OPTION 2: 
    // std::unique_ptr<ChatBot> chatBot(std::make_unique<ChatBot>("../images/chatbot.png"));
    // NOT an option because requirement states: 
    // =>  "In file chatlogic.cpp, a local ChatBot instance is created on the stack 
    //      at the bottom of function LoadAnswerGraphFromFile and move semantics are 
    //      used to pass the ChatBot instance into the root node."
    // If a unique_ptr was used the move semantics of the object would not be required
    //=============================================================================

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    chatBot.SetChatLogicHandle(this);

    // add chatbot to graph root node
    chatBot.SetRootNode(rootNode); 

    rootNode->MoveChatbotHere(std::move(chatBot));

    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
