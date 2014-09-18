#include "provided.h"
#include "http.h"
#include <string>
#include <queue>
using namespace std;

class WebCrawlerImpl
{
public:
    void addUrl(string url);
    int getNumberOfUrls() const;
    void crawl(void (*callback)(string url, bool success));
    bool save(string filenameBase);
    bool load(string filenameBase);
private:
    queue<string> m_webCrawl;
    Indexer m_indexer;
};

void WebCrawlerImpl::addUrl(string url)
{
    m_webCrawl.push(url);
//    This method must simply store its argument URL in some sort of container (and not crawl it) until such time that the user of the class calls the crawl() method.
}

int WebCrawlerImpl::getNumberOfUrls() const
{
    return m_webCrawl.size();
//    The getNumberOfUrls() method returns the number of URLs that have been added to the WebCrawler object by addURL() but have not yet been processed by crawl().
}

void WebCrawlerImpl::crawl(void (*callback)(string url, bool success))
{
    string page;
    while (!m_webCrawl.empty()) {
        if (HTTP().get(m_webCrawl.front(), page)) {
            WordBag newWordBag(page);
            m_indexer.incorporate(m_webCrawl.front(), newWordBag);
            callback(m_webCrawl.front(), true);
        }
        else callback(m_webCrawl.front(), false); // is this all I do for the callback?
        m_webCrawl.pop();
    }
//    The crawl() method is responsible for iterating through all of the URLs added to a WebCrawler object, and does the following for each:
//        1. Connect to the website and download the web page at the specified URL
//        2. If the download was successful:
//        a. Place the web page into a WordBag object
//        b. Incorporate the WordBag into the WebCrawler's Indexer.
//        3. Call a “callback” function, provided by the user via a function pointer, to report
//        the status of the web page download.
}

bool WebCrawlerImpl::save(string filenameBase)
{
    return m_indexer.save(filenameBase);
//    The save() method saves the index built by a WebCrawler object to one or more files on disk so that the index can be loaded and used at a later time. The file(s) have names beginning with the filenameBase string passed as the argument. It returns true if the index was saved successfully, false otherwise.
}

bool WebCrawlerImpl::load(string filenameBase)
{
    return m_indexer.load(filenameBase);
//    The load() method loads a previously-saved index into a WebCrawler object. Whatever was previously indexed by the crawler is discarded, so that after the load, the crawler contains the same content that the crawler that saved the index had at the time of the save. The file(s) containing the previously-saved index to be loaded have names beginning with the filenameBase string passed as the argument. It returns true if the index was loaded successfully, false otherwise.
}

//******************** WebCrawler functions *******************************

// These functions simply delegate to WebCrawlerImpl's functions.
// You probably don't want to change any of this code.

WebCrawler::WebCrawler()
{
    m_impl = new WebCrawlerImpl;
}

WebCrawler::~WebCrawler()
{
    delete m_impl;
}

void WebCrawler::addUrl(string url)
{
    m_impl->addUrl(url);
}

int WebCrawler::getNumberOfUrls() const
{
    return m_impl->getNumberOfUrls();
}

void WebCrawler::crawl(void (*callback)(string url, bool success))
{
    m_impl->crawl(callback);
}

bool WebCrawler::save(string filenameBase)
{
    return m_impl->save(filenameBase);
}

bool WebCrawler::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
