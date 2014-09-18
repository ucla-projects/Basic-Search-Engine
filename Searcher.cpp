#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class SearcherImpl
{
public:
    vector<string> search(string terms);
    bool load(string filenameBase);
private:
    struct newPair
    {
        newPair()
        {
            m_unique = 1;
            m_count = 0;
        }
        int m_unique;
        int m_count;
    };
    
    Indexer m_index;
    MyMap<string, newPair> urlScore; // map that contains URL and newPair (unique count, and count)
};

bool CompareCount(const UrlCount &A, const UrlCount &B)
{
    if (A.count >= B.count)
        return true;
    return false;
}

vector<string> SearcherImpl::search(string terms)
{
    strToLower(terms);
    string w;
    Tokenizer x(terms);
    int countOfWords = 0;
    while (x.getNextToken(w)) countOfWords++; // countOfWords has the number of words in "terms"
    int T;
    if (int(0.7*countOfWords) > 1) T = int(0.7*countOfWords);
    else T = 1;
    
    while (x.getNextToken(w)) {
        vector<UrlCount> urltoCount = m_index.getUrlCounts(w); // holds all the URLs and counts
        for (int i=0; i<urltoCount.size(); i++) {
            newPair* x = urlScore.find(urltoCount[i].url);
            if (x == nullptr) {
                newPair y;
                y.m_unique = 1;
                y.m_count = 1;
                urlScore.associate(urltoCount[i].url, y);
            }
            else { // the URL already exists in the urlScore map
                x->m_unique++;
                x->m_count += urltoCount[i].count;
            }
        }
    }
    
    string a; // push the elements of the map onto the vector
    newPair* hold = urlScore.getFirst(a);
    vector<UrlCount> newUrltoCount;
    while (hold != nullptr) {
        if (hold->m_unique > T) {
            UrlCount x;
            x.url = a;
            x.count = hold->m_count;
            newUrltoCount.push_back(x);
            
        }
        else hold = urlScore.getNext(a);
    }
    
    sort(newUrltoCount.begin(), newUrltoCount.end(), CompareCount); // newUrlToCount is sorted
    
    vector<string> m_sortedURL;
    for (int i = 0; i < newUrltoCount.size(); i++) {
        m_sortedURL.push_back(newUrltoCount[i].url);
    }
    
    return m_sortedURL;
//    The search() method allows the user to search the index for documents that match a specified set of search terms. The method returns a vector of zero or more items containing URLs whose contents matched the search terms according to our specified matching algorithm detailed below.
        
//        All searches conducted by search() must be case insensitive (that is, a search for “ZITS” and “ziTs” and “zits” must all produce the same results; you must not assume the search terms will be all lower case). Use only the words in the terms string (hint: Tokenizer!). Treat a word appearing more than once in the terms string as if it appears only once.
    // page 19
}

bool SearcherImpl::load(string filenameBase)
{
    return m_index.load(filenameBase);
//    The load() method loads a previously-saved index into a Searcher object. Whatever was previously in the Searcher object's index is discarded, so that after the load, the Searcher contains the same content that was saved to the index. The file(s) containing the previously-saved index to be loaded have names beginning with the filenameBase string passed as the argument. It returns true if the index was loaded successfully, false otherwise.
}

//******************** Searcher functions *******************************

// These functions simply delegate to SearcherImpl's functions.
// You probably don't want to change any of this code.

Searcher::Searcher()
{
    m_impl = new SearcherImpl;
}

Searcher::~Searcher()
{
    delete m_impl;
}

vector<string> Searcher::search(string terms)
{
    return m_impl->search(terms);
}

bool Searcher::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
