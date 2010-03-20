import os
import re
from dictd_index import Complete, cook_raw_dictionary


file_path = "/home/dim/Lang/chn/cedict_ts.u8"
dic_name = "cedict"

number_re = re.compile("\d+")
def remove_tome_indices( pinyin ):
    return re.sub( number_re, "", pinyin)

def read_header( stream ):
    disclaimer = list()
    while True:
        line = stream.readline()
        if line.startswith("#"):
            disclaimer.append( line )
            continue
        else:
            break
            
    return "".join( disclaimer )
        
def splitmany(string, splitby):
    if not splitby:
        return [string]
    rval = list()
    for item in string.split(splitby[0]):
        rval.extend( splitmany( item, splitby[1:]))
    return rval
    
    

item_re = re.compile(ur"^(\S+)\s(\S+)\s\[([^\]]+)\]\s")
def parse_raw( stream ):
    #xxx xxx [A1 bo1 luo2] /Apollo/
    item_begin = stream.tell()    
    while True:
        line = stream.readline()
        if not line:
            raise Complete
        if line.startswith("#"):
            print line
            continue
        try:
            lineu = line.decode("utf-8")
        except Exception, e:
            print "Failed to decode line, skipping"
            continue
        m=item_re.match( lineu )
        if m:
            simp = m.group(1).encode("utf-8")
            trad = m.group(2).encode("utf-8")
            pinyin = m.group(3).encode("utf-8")
            pinyin_notone = remove_tome_indices( pinyin )
            print "####", simp, trad, pinyin, pinyin_notone
            if trad==simp:
                hanzi = [trad]
            else:
                hanzi = [trad, simp]
            return hanzi+[pinyin, pinyin_notone], item_begin, stream.tell()
        else:
            print "Skipped line:", line

if __name__=="__main__":
    print "Converting data"
    cook_raw_dictionary(read_header, parse_raw, file_path, dic_name)
    print "Done"
    
