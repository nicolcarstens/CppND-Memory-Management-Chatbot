#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();                    // destructor => 1 of Rule of 5 

    //// STUDENT CODE - Task 2: The Rule of Five
    ////

    // Copy Constructor - 2 of Rule of 5 
    ChatBot(const ChatBot& source);
    // Copy Assignment Operator - 3 of Rule of 5 
    ChatBot& operator=(const ChatBot& source);
    // Move Constructor - 4 of Rule of 5 
    ChatBot(ChatBot&& source);
    // Move Assignment Operator - 5 of Rule of 5 
    ChatBot& operator=(ChatBot&& source);
 
    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */