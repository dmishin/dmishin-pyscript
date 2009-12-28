#warodai parsing 
import os
from dictd_index import Complete, cook_raw_dictionary

#line 319554: remove <i>

file_path = "warodai.txt"
dic_name = "warodai"

lat_numerics="IVX"

def remove_lat_numeral(s):#remove lating numeral from the end of the string
    removed = 0
    while len(s)>0 and s[-1]in lat_numerics and removed<4:
        s=s[:-1]
        removed+=1
    return s


def read_header( stream ):
    is_reading_disclaimer = False
    disclaimer_text=[]
    while True:
        line = stream.readline()
        if line.startswith("***"):
            is_reading_disclaimer = not is_reading_disclaimer
            continue
            
        if is_reading_disclaimer:
            disclaimer_text.append( line )
            if len(disclaimer_text)>1000:
                raise ValueError, "Error! disclaimer too big?"
            continue
            
        if line=="\n":
            break
            
    return "".join( disclaimer_text )
        
def splitmany(string, splitby):
    if not splitby:
        return [string]
    rval = list()
    for item in string.split(splitby[0]):
        rval.extend( splitmany( item, splitby[1:]))
    return rval
    
    
def parse_raw( stream ):
    item_begin = stream.tell()    
    while True:
        line = stream.readline()
        #current_pos += len(line)
        if line =="\n": continue
        break
    if not line: raise Complete
    
    ru_yomi_begin = line.index("(")
    ru_yomi_end = line.index(")", ru_yomi_begin)
    
    yomi_kaki = line[:ru_yomi_begin]
    
    kaki_ari = "\xe3\x80\x90" in line
    if kaki_ari:
        kaki_begin = line.index("\xe3\x80\x90")
        kaki_end = line.index("\xe3\x80\x91")
        yomi = line[:kaki_begin]
        kaki = line[kaki_begin+3:kaki_end]
    else:#kaki_nashi
        yomi = yomi_kaki
        kaki = None
    
    yomi = splitmany(yomi, (", ",))
    if kaki:
        kaki = splitmany(kaki, ("\xef\xbd\xa5", ", ") )
        
    yomikaki = yomi+kaki if kaki else yomi
    
    while True: #skip the definition text
        line = stream.readline()
        if line == "\n" or not line:
            break
    item_end = stream.tell() - 1    
    return [remove_lat_numeral(item.strip()) for item in yomikaki], item_begin, item_end

if __name__=="__main__":
    print "Converting data"
    cook_raw_dictionary(read_header, parse_raw, file_path, dic_name)
    print "Done"
    
