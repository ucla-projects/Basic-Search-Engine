#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include <queue>
#include <string>

using namespace std;

template <class KeyType, class ValueType>
struct Node
{
    Node(KeyType key, ValueType value, Node *left, Node *right)
    {
        m_key = key;
        m_value = value;
        leftChild = left;
        rightChild = right;
    }
    KeyType m_key;
    ValueType m_value;
    Node *leftChild, *rightChild;
};

template <class KeyType, class ValueType>
class MyMap
{
public:
    MyMap()
    {
        m_head = nullptr;
        m_numAssoc = 0;
        //	constructor
    }
    
    ~MyMap()
    {
        //	destructor;	deletes	all	of the trees nodes
        clear();
    }
    
    void clear()
    {
        //	deletes	all	of	the	tree nodes -> empty tree
        if (m_head == nullptr)
            return;
        Node<KeyType, ValueType>* curNode = m_head;
        helpClear(curNode);
    }
    
    int size() const
    {
        // return the number of associations in the map
        return m_numAssoc;
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        // The associate method associates one item (key) with	another	(value). If no association currently exists with that key, this method inserts a new association into the tree with that key/value pair. If there is already an association with that key in the tree, then the item associated with that key is replaced by the second parameter (value). Thus, the tree contains no duplicate keys.
        if (m_head == nullptr) { // tree is empty
            m_head = new Node<KeyType, ValueType>(key, value, nullptr, nullptr);
            m_numAssoc = 1;
            queueOfNodes.push(m_head);
            return;
        }
        Node<KeyType, ValueType>* curNode = m_head;
        while (curNode != nullptr) {
            if (curNode->m_key == key) { // key is equal to current node's key
                curNode->m_value = value;
                return;
            }
            if (key < curNode->m_key) { // key is less than current node's key
                if (curNode->leftChild != nullptr) curNode = curNode->leftChild;
                else {
                    curNode->leftChild = new Node<KeyType, ValueType>(key, value, nullptr, nullptr);
                    m_numAssoc++;
                }
                continue;
            }
            if (key > curNode->m_key) { // key is greater than current node's key
                if (curNode->rightChild != nullptr) curNode = curNode->rightChild;
                else {
                    curNode->rightChild = new Node<KeyType, ValueType>(key, value, nullptr, nullptr);
                    m_numAssoc++;
                }
                continue;
            }
        }
    }
    
    const ValueType* find(const KeyType& key) const
    {
        // If no association exists with the given key, return NULL; otherwise, return a pointer to value associated with that key. This pointer can be used to examine that value, and if the map is allowed to be modified, to modify that value directly within the map. (The second overload enables this. Using a little C++ magic, we have implemented it in terms of the first overload, which you must implement.)
        if (m_head == nullptr)
            return nullptr;
        Node<KeyType, ValueType>* curNode = m_head;
        const ValueType* returnValue = helpFind(key, curNode);
        return returnValue;
    }
    
    ValueType* find(const KeyType& key)
    {
        // Do not change the implementation of this overload of find
        const MyMap<KeyType,ValueType>* constThis = this;
        return const_cast<ValueType*>(constThis->find(key));
    }
    
    ValueType* getFirst(KeyType& key)
    {
        if (m_head == nullptr) return nullptr; // if tree is empty
        Node<KeyType, ValueType>* temp = queueOfNodes.front();
        queueOfNodes.pop();
        key = temp->m_key;
        if (temp->leftChild != nullptr) queueOfNodes.push(temp->leftChild);
        if (temp->rightChild != nullptr) queueOfNodes.push(temp->rightChild);
        ValueType* tempVal = &(temp->m_value);
        return tempVal;
    }
    
    ValueType* getNext(KeyType& key)
    {
        if (queueOfNodes.empty()) return nullptr;
        Node<KeyType, ValueType>* temp = queueOfNodes.front();
        queueOfNodes.pop();
        key = temp->m_key;
        if (temp->leftChild != nullptr) queueOfNodes.push(temp->leftChild);
        if (temp->rightChild != nullptr) queueOfNodes.push(temp->rightChild);
        ValueType* tempVal = &(temp->m_value);
        return tempVal;
        // must run in constant time (its runtime must not be proportional to the number of nodes in the tree)
        //return nullptr; // if getNext() is called after all associations have been retrieved
    }
    
private:
    MyMap(const MyMap &other);
    MyMap &operator=(const MyMap &other);
    Node<KeyType, ValueType>* m_head;
    int m_numAssoc;         // number of associations
    queue<Node<KeyType, ValueType>*> queueOfNodes;
    
    // helper functions
    void helpClear(Node<KeyType, ValueType>* currentNode)
    {
        if (currentNode == nullptr) return;
        helpClear(currentNode->leftChild);
        helpClear(currentNode->rightChild);
        delete currentNode;
    }
    
    const ValueType* helpFind(const KeyType& key, Node<KeyType, ValueType>* currentNode) const
    {
        while (currentNode != nullptr) {
            if (currentNode->m_key == key) return &(currentNode->m_value);
            if (key < currentNode->m_key) return helpFind(key, currentNode->leftChild);
            if (key > currentNode->m_key) return helpFind(key, currentNode->rightChild);
        }
        return nullptr;
    }
};

#endif // MYMAP_INCLUDED
