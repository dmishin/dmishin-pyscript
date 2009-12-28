
import os

b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

class Complete (Exception):
    pass

def b64_encode(num):
    rval = []
    while num >0 :
        x = num%64
        num /= 64
        rval.insert(0, b64_table[x] )
    if not rval:
        return b64_table[0]
    else:
        return "".join(rval)

def cook_raw_dictionary(header_parser, item_parser, source_file, dic_name, create_dzip=True, dzip_program="dictzip"):
    "header-perser must return parsed header string. item parser returns tuples of :(<kword lilst>, begin index, end index)"
    stream = open( source_file, "rb")
    
    print "Readign header"
    header = header_parser( stream )
    
    print "Parsing raw items"
    
    items = list()

    num_items = 0
    while True:
        try:
            yomikaki, begin, end = item_parser( stream )        
            num_items += 1
            for item in yomikaki:
                items.append( (item, begin, end) )
        except Complete:
            break
    stream.close()
    
    print "Done. Read %d dictionary entriex, %d index records created"%(num_items, len(items))
    print "Sorting index",
    items.sort(key = lambda item:item[0])
    print "Done."
    
    index_file_name = "%s.index"%dic_name
    
    print "Writing index %s"%index_file_name,
    index_file = open(index_file_name, "w")
    for word, begin, end in items:
        index_file.write( "%s\t%s\t%s\n"%(\
            word, \
            b64_encode( begin ), \
            b64_encode( end-begin )))
    index_file.close()
    print "Done"

    if create_dzip:
        if not os.path.exists("%s.dict.dz"%(dic_name)):
            print "Creating data file:"
            os.system("cp %s %s.dict"%(source_file, dic_name))
            print "Zipping..."
            os.system("%s %s.dict"%(dzip_program, dic_name))
        else:
            print "Data file %s already exists"%("%s.dict.dz"%(dic_name))
        

