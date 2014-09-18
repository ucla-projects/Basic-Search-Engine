#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class WordBagImpl
{
public:
    WordBagImpl(const string& text);
    bool getFirstWord(string& word, int& count);
    bool getNextWord(string& word, int& count);
private:
    MyMap<string, int> m_map;
    Node<string, int>* m_root;
};

WordBagImpl::WordBagImpl(const string& text)
{
    Tokenizer t(text);
    string w;
    while (t.getNextToken(w)) {
        strToLower(w);
        if (m_map.size() == 0) {
            m_map.associate(w, 1); // Map was empty
            continue;
        }
        int* temp = nullptr;
        temp = m_map.find(w);
        if (temp == nullptr) m_map.associate(w, 1); // wasn't already in map
        else m_map.associate(w, *(temp) + 1); // was already in map
    }
    // The constructor must break the string up into its constituent words, discarding all whitespace and punctuation marks.
}

bool WordBagImpl::getFirstWord(string& word, int& count)
{
    int* temp = m_map.getFirst(word);
    if (temp == nullptr) return false; // tree is empty
    count = *temp;
    return true;
}

bool WordBagImpl::getNextWord(string& word, int& count)
{
    int* temp = m_map.getNext(word);
    if (temp == nullptr) return false; // check is whole tree has been traversed
    count = *temp;
    return true;
}

//******************** WordBag functions *******************************

// These functions simply delegate to WordBagImpl's functions.
// You probably don't want to change any of this code.

WordBag::WordBag(const std::string& text)
{
    m_impl = new WordBagImpl(text);
}

WordBag::~WordBag()
{
    delete m_impl;
}

bool WordBag::getFirstWord(string& word, int& count)
{
    return m_impl->getFirstWord(word, count);
}

bool WordBag::getNextWord(string& word, int& count)
{
    return m_impl->getNextWord(word, count);
}
