import xml.sax.handler
import StringIO

class object_builder:
    def __init__(self):
        self.object = None
        self.creator = None
        self.finalizer = None
        self.handlers=dict() #name -> (parser, receiver)
        
    def characters(self, chars):
        pass
    def create(self, attributes):
        self.object = self.creator(attributes)

    def finish(self):
        if self.finalizer:
            self.finalizer(self.object)
        return self.object
        
    def receive_value(self, tag, value):
        "receive the value,m returned by sub-handler"
        self.handlers[tag][1](tag, self.object, value)
        
    def get_tag_parser(self, tag, attributes=None):
        "Get the hadler for the tag"
        try:
            return self.handlers[tag][0]
        except KeyErrror, e:
            return None
    
    def set_handler(self, tag_name, tag_parser, tag_receiver):
        self.handlers[tag_name] = (tag_parser, tag_receiver)
        
    def del_handler(self, tag_name):
        del self.handlers[tag_name]
        
            
class xml_handler(xml.sax.handler.ContentHandler):
    def __init__(self, initial_parser):
       self.parsers=[initial_parser]
 
    def cur_parser(self):
        return self.parsers[-1]
        
    def startElement(self, name, attributes):
        handler = self.cur_parser().get_tag_parser(name, attributes)
        self.parsers.append(handler)
        handler.create(attributes)
        
    def characters(self, data):
        self.cur_parser().characters(data)
 
    def endElement(self, name):
        obj = self.cur_parser().finish()
        del self.parsers[-1]
        self.cur_parser().receive_value(name, obj)

test_xml="""
<item>
  <sub-item name="a"/>
  <sub-item name="b"/>
</item>
"""

def root_builder(root_tag, tag_parser):
    builder = object_builder()
    def receive_func(tag, none_object, parsed):
        builder.object = parsed
    builder.set_handler(root_tag, tag_parser, receive_func)
    return builder
    
def test():
    
    sub_item = object_builder()
    sub_item.creator = lambda attrs: [attrs.get("name","unnamed")]
    class Item:
        def __init__(self):
            self.sub_items=[]
        def __str__(self):
            return "Items:%s"%(self.sub_items)
        def add(self, itm):
            self.sub_items.append(itm)

        
    item = object_builder()
    item.creator = lambda attrs: Item()
    item.set_handler("sub-item", sub_item, (lambda tag, item, sub_item: item.add(sub_item)))
    
    
    
    parser = xml.sax.make_parser()
    rootbld = root_builder("item", item)
    handler = xml_handler(rootbld)
    parser.setContentHandler(handler)
    parser.parse(StringIO.StringIO(test_xml))
    
    print rootbld.object

test()
