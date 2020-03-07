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
*  TASK 2: The Rule of Five
*
*  In file chatbot.h / chatbot.cpp, changes are made to the class ChatBot 
*  such that it complies with the Rule of Five. Memory resources are 
*  properly allocated / deallocated on the heap and member data is copied 
*  where it makes sense. In each of the methods (e.g. the copy constructor), 
*  a string of the type "ChatBot Copy Constructor" is printed to the console 
*  so that it is possible to see which method is called in later examples.
*
******************************************************************************/

#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

// CONSTRUCTOR WITHOUT memory allocation - 0 of 5
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = nullptr; 
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// CONSTRUCTOR WITH memory allocation - 0 of 5
ChatBot::ChatBot(std::string filename)
{
    std::cout << "ChatBot Constructor" << std::endl;
    
    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

// DESTRUCTOR - 1 of 5 
ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;

    // deallocate heap memory
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;  // NC NOTE: only delete what you own! 
        _image = NULL;
    }
}

//// STUDENT CODE - Task 2: The Rule of Five
////

// COPY CONSTRUCTOR - 2 of 5 
ChatBot::ChatBot(const ChatBot& source)
{
    std::cout << "ChatBot Copy Constructor " << std::endl;

    //_image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
    _image = new wxBitmap();

    // you want to copy the data, not the pointer - deep copy! 
    *_image = *source._image;    

    // copy pointers - could be a NULL/nullptr or a non-NULL/nullptr
    _currentNode = source._currentNode;
    _rootNode    = source._rootNode;
    _chatLogic   = source._chatLogic;

    // In the case where you are removing the source ChatBot, makes sense
    // to transfer the SetChatbotHandle... but what about Copy cases?
    // Should not need to copy the ChatBot? Only one ever created. Unique.
    _chatLogic->SetChatbotHandle(this);    
}

// COPY ASSIGNMENT OPERATOR - 3 of 5 
ChatBot& ChatBot::operator=(const ChatBot& source)
{
    std::cout << "ChatBot Copy Assignment Operator " << std::endl;      

    // don't copy to yourself ... 
    if (this == &source)
        return *this;

    // Delete what you own (_image data on heap) - this is not a constructor - 
    // But only if something was created! 
    // ChatBot could have been constructed without assignment 
    if (_image != NULL){
        delete _image;
    } 

    //_image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
    _image = new wxBitmap();

    // you want to copy the data, not the pointer - deep copy! 
    *_image = *source._image;    

    // copy pointers - could be a NULL/nullptr or a non-NULL/nullptr
    _currentNode = source._currentNode;
    _rootNode    = source._rootNode;
    _chatLogic   = source._chatLogic;

    // In the case where you are removing the source ChatBot, makes sense
    // to transfer the SetChatbotHandle... but what about Copy cases?
    // Should not need to copy the ChatBot? Only one ever created. Unique.    
    _chatLogic->SetChatbotHandle(this);

    return *this;
}

// MOVE CONSTRUCTOR - 4 of 5 
ChatBot::ChatBot(ChatBot&& source)
{
    std::cout << "ChatBot Move Constructor " << std::endl;

    // copy pointers - could be a NULL/nullptr or a non-NULL/nullptr
    _image       = source._image;        
    _currentNode = source._currentNode;
    _rootNode    = source._rootNode;
    _chatLogic   = source._chatLogic;

    // In the case where you are removing the source ChatBot, 
    // makes sense to transfer the SetChatbotHandle.
    _chatLogic->SetChatbotHandle(this); 

    source._image       = NULL;     // NULL => old wxWidgets specific      
    source._currentNode = nullptr;  // modern nullptr
    source._rootNode    = nullptr;
    source._chatLogic   = nullptr;
}

// MOVE ASSIGNMENT OPERATOR - 5 of 5 
ChatBot& ChatBot::operator=(ChatBot&& source)
{
    std::cout << "ChatBot Move Assignment Operator " << std::endl;

    // don't move to yourself ... 
    if (this == &source)
        return *this;

    // Delete what you own (_image data on heap) - this is not a constructor - 
    // But only if something was created! 
    // ChatBot could have been constructed without assignment 
    if (_image != NULL){
        delete _image;
    }
    
    // copy pointers - could be a NULL/nullptr or a non-NULL/nullptr
    _image       = source._image;        
    _currentNode = source._currentNode;
    _rootNode    = source._rootNode;
    _chatLogic   = source._chatLogic;

    // In the case where you are removing the source ChatBot, makes sense
    // to transfer the SetChatbotHandle.
    _chatLogic->SetChatbotHandle(this);   

    source._image       = NULL;     // NULL => old wxWidgets specific
    source._currentNode = nullptr;  // modern nullptr
    source._rootNode    = nullptr;
    source._chatLogic   = nullptr;

    return *this;
}

////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}