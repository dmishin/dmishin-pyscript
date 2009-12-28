#chan-parser

from urllib2 import urlopen, URLError, HTTPError
import BeautifulSoup as bs


def get_soup(soup, 

def parse_page(page):
    soup = bs.BeautifulSoup(html)
    threads_container = soup.find(name="form", attrs={"id":"delform"})
    #enumerate all child tags
    for child in threads_container.contents:
        if not isinstance(child, bs.Tag):
            continue

        if child.name != "div": continue
        threadid = child.get("id", None)
        if threadid == None:    continue

        
    


data = urlopen("http://iichan.ru/tran/")

html = data.read()

data.close()

parse_page(html)
