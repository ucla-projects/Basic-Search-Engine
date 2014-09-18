#include "provided.h"
#include <string>
#include <vector>
#include "MyMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Pair
{
    Pair(int ID, int count)
    {
        m_ID = ID;
        m_count = count;
    }
    int m_ID;
    int m_count;
};

class IndexerImpl
{
public:
    bool incorporate(string url, WordBag& wb);
    vector<UrlCount> getUrlCounts(string word);
    bool save(string filenameBase);
    bool load(string filenameBase);
private:
    MyMap<string, int> urlToId;
    MyMap<int, string> idToUrl;
    MyMap<string, vector<Pair> > wordToIdCounts;
};

bool IndexerImpl::incorporate(string url, WordBag& wb)
{
    // adding to urlToId map, adding to idToUrl map
    int* temp = urlToId.find(url);
    if (temp != nullptr) return false;
    
    string word;
    int retrieveCount;
    bool getCount = wb.getFirstWord(word, retrieveCount);
    if (getCount == false) return false;
    
    int ID = rand() %100000;
    while (idToUrl.find(ID) != nullptr) { ID = rand() %100000; }
    urlToId.associate(url, ID); // add to urlToId map
    idToUrl.associate(ID, url); // add to idToUrl map
    
    while (getCount) {
        if (wordToIdCounts.find(word) != nullptr) {
            vector<Pair> *x = wordToIdCounts.find(word);
            x->push_back(Pair(ID, retrieveCount));
        }
        else {
            vector<Pair> y;
            y.push_back(Pair(ID, retrieveCount));
            wordToIdCounts.associate(word, y);
        }
        getCount = wb.getNextWord(word, retrieveCount);
    }
    return true;
//    The user calls the incorporate() method to add all the words in the provided WordBag argument to the index. If incorporate() has previously been called with the same url argument, it returns false, doing nothing. Otherwise, it updates the index by adding all the words and returns true. Incorporating a WordBag containing W distinct words to an index that already contains N words must take far less than O(WN) time, because adding one word mapping (e.g., "fun" → { "www.b.com", 1 }) to an index that already contains N words must take far less than O(N) time.
}

vector<UrlCount> IndexerImpl::getUrlCounts(string word)
{
    vector<UrlCount> x;
    strToLower(word);
    if (wordToIdCounts.find(word) == nullptr) return x;
    vector<Pair> y = *wordToIdCounts.find(word);
    for (int i=0; i<y.size(); i++) {
        string Url = *(idToUrl.find(y[i].m_ID));
        int tempCount = y[i].m_count;
        UrlCount temp;
        temp.url = Url;
        temp.count = tempCount;
        x.push_back(temp);
    }
    return x;
    
//    The getUrlCounts() method returns a vector of UrlCounts for the specified word. The getUrlCounts() method must be case-insensitive, so it must return the same result for, e.g., "zits", "ZITS", or "ZitS" as the word argument. There is no requirement that the UrlCounts in the vector returned by this method be in any particular order. Each UrlCount in the vector holds a URL string and the count associated with that word in the WordBag incorporated with that URL (representing the number of times that word appeared on the web page at that URL). A given URL string must occur only once within the vector of matches. If your index contains N words, your getUrlCounts() method must return a vector of UrlCounts in far less than O(N) time.
}

void writeItem(ostream& stream, string s) { stream << s << endl; }

void writeItem(ostream& stream, int i) { stream << i << endl; }

void writeItem(ostream& stream, const Pair& c) { stream << c.m_ID << endl; stream << c.m_count << endl; }

void writeItem(ostream& stream, const vector<Pair>& lc) // for map value
{
    vector<Pair>::const_iterator ptr;
    for (ptr = lc.begin(); ptr != lc.end(); ptr++) {
        writeItem(stream, ptr->m_ID);
        writeItem(stream, ptr->m_count);
    }
}

template<typename KeyType, typename ValueType>
bool saveMyMap(string filename, MyMap<KeyType, ValueType>& m)
{
    // save contents of m to a file
    ofstream stream("filename");    // Create an ofstream object to create the file
    if (! stream) return false;     // Return false if we can't create the file
    writeItem(stream, m.size());    // Save the number of associations in m to stream
    KeyType x;
    ValueType* y = m.getFirst(x);
    for (int i = 0; i < m.size(); i++) {
        writeItem(stream, x);
        writeItem(stream, *y);
        y = m.getNext(x);
    }
    return true;
}

bool IndexerImpl::save(string filenameBase)
{
    return saveMyMap(filenameBase + ".u2i", urlToId) &&
    saveMyMap(filenameBase + ".i2u", idToUrl) &&
    saveMyMap(filenameBase + ".w2ic", wordToIdCounts);
    //    The save() method saves the index that an Indexer object has built to one or more files on disk so that the index can be loaded and used at a later time. The file(s) have names beginning with the filenameBase string passed as the argument. It returns true if the index was saved successfully, false otherwise. See below for details about this method.
}

bool readItem(istream& stream, string& s)
{
    return getline(stream, s);
}
bool readItem(istream& stream, int& i)
{
    string line;
    if (!getline(stream, line)) return false;
    istringstream iss(line);
    return iss >> i;
    
}

bool readItem(istream& stream, vector<Pair>& v)
{
    int newSize = v.size();
    readItem(stream, newSize);
    v.clear();
    for (int i = 0; i < newSize; i++) {
        int a, b;
        if (!readItem(stream, a)) return false;
        if (!readItem(stream, b)) return false;
        Pair p(a, b);
        v.push_back(p);
    }
    return true;
}
template<typename KeyType, typename ValueType>
bool loadMyMap(string filename, MyMap<KeyType, ValueType>& m)
{
    m.clear();
    ifstream stream("filename");
    if (!stream) return false;
    int size = m.size();
    if (!readItem(stream, size)) return false; // Read the number of associations in m from stream, returning false if we can't
    KeyType x;
    ValueType y;
    for (int i = 0; i < size; i++) {
        if (!readItem(stream, x)) return false;
        stream.ignore(10000, '\n');
        if (!readItem(stream, y)) return false;
        stream.ignore(10000, '\n');
        m.associate(x, y);
    }
    return true;
}

bool IndexerImpl::load(string filenameBase)
{
    return loadMyMap(filenameBase + ".u2i", urlToId) &&
    loadMyMap(filenameBase + ".i2u", idToUrl) &&
    loadMyMap(filenameBase + ".w2ic", wordToIdCounts);
    //return false;
//    The load() method loads a previously-saved index into an Indexer object. The previous contents of the Indexer object are discarded, so that after the load, the Indexer contains the same content that the Indexer that saved the index had at the time of the save. The file(s) containing the previously-saved index to be loaded have names beginning with the filenameBase string passed as the argument. It returns true if the index was loaded successfully, false otherwise. See below for details about this method.
}

//******************** Indexer functions *******************************

// These functions simply delegate to IndexerImpl's functions.
// You probably don't want to change any of this code.

Indexer::Indexer()
{
    m_impl = new IndexerImpl;
}

Indexer::~Indexer()
{
    delete m_impl;
}

bool Indexer::incorporate(string url, WordBag& wb)
{
    return m_impl->incorporate(url, wb);
}

vector<UrlCount> Indexer::getUrlCounts(string word)
{
    return m_impl->getUrlCounts(word);
}

bool Indexer::save(string filenameBase)
{
    return m_impl->save(filenameBase);
}

bool Indexer::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
