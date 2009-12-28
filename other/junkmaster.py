#!/usr/bin/python

import sys
import sqlite3 as sql
import getopt
import os
import tempfile

#path to the database file
default_home_dir = "e:\\"
db_path = os.path.join(os.environ.get("HOME", default_home_dir), ".junkmaster.db") 
connection = sql.connect(db_path)
cursor = connection.cursor()



editor = "vi"
encoding = "utf-8"

def create_tables():
    print "Creating tables...",
    #table for storing basic file data
    cursor.execute("""
        create table if not exists junk (
            id integer primary key, 
            name text, 
            folder text, 
            size text, 
            hash text,
            unique (name, folder) on conflict fail
        )
    """)
    #table for tags
    cursor.execute("""
        create table if not exists tags (
            id integer primary key,
            name text unique on conflict fail
        )
    """)
    
    #table for tag values
    #table for tags
    cursor.execute("""
        create table if not exists file_tags (
                idfile integer,
                idtag integer,
                value null,
                unique(idfile, idtag) on conflict replace,
                foreign key (idfile) references junk(id),
                foreign key (idtag)  references tags(id)
            )
    """)
    
    #create the default tags
    create_tag("desc", False)
    connection.commit()
    print "Done"
    
def create_tag(tagname, commit = True):
    "Create new tag"
    try:
        cursor.execute("insert into tags(name) values (?)", (tagname,))
    except sql.IntegrityError, e:
        pass
        
    if commit:
        connection.commit()
    
def add_file(fname, commit = True):
    "Add file to the database"
    abs_path = os.path.abspath(fname)
        
    folder,name = os.path.split(abs_path)
    size = os.path.getsize(abs_path)
    
    try:
        cursor.execute("insert into junk (name, folder, size) values(?, ?, ?)", (name, folder, size))
    except sql.IntegrityError, e:
        print "File %s already present in the database"%fname
        return False
        
    if commit:
        connection.commit()


def check_present(apath):
    return get_id(apath) != None
    
def get_id(apath):
    folder, name = os.path.split(apath)
    
    #exact match
    id_tuple = cursor.execute("select id from junk where name=? and folder=?",(name, folder)).fetchone()

    if id_tuple == None:
        return None
    else:
        return id_tuple[0]
    
def show_help():
    print "junkmaster command ...."


def show_info(fname):
    apath = os.path.abspath(fname)
    folder, name = os.path.split(apath)
    
    info = cursor.execute("select id, size from junk where name=? and folder=?", (name, folder)).fetchone()
    
    if info == None:
        print "File %s not present in the DB"%fname
        return
    
    id, sz = info
    print "File %s ID:%s size:%s"%(fname, id, sz)
    
        
    
def get_tags(file_id):
    "Returns list of tags"
    pass

def add_tags(file_id, taglist, commit=True):
    pass
    if commit:
        connection.commit()

def remove_tags(file_id, taglist, commit = True):
    "Remove tags from file"
    pass
    if commit:
        connection.commit()

    
def get_tag(file_id, tag_id):
    "Return the description"
    row = cursor.execute("select (value) from file_tags where idfile=? and idtag=?",(file_id, tag_id)).fetchone()
                       
    if row == None:#nothing fetched
        return None
    return row[0]



def get_tag_id(tagname, create=True):
    tagids = cursor.execute("select (id) from tags where name=?", (tagname,)).fetchone()
    if tagids == None: #tag not exists
        create_tag(tagname, True)
        return get_tag_id(tagname, False)
    else:
        return tagids[0]    
    
def set_tag(file_id, tagid, tagval, commit=True):
    "Set the tag value"
    cursor.execute("insert into file_tags(idfile, idtag, value) values (?, ?, ?)", (file_id, tagid, tagval))
    if commit:
        connection.commit()

def set_desc(file_id, desc, commit=True):
    return set_tag(file_id, get_tag_id("desc"), desc, commit)
    
def get_desc(file_id):
    return get_tag(file_id, get_tag_id("desc"))

    

def get_id(abs_path):
    folder, name = os.path.split(abs_path)
    row = cursor.execute("select id from JUNK where name=? AND folder=?",(name, folder)).fetchone()
    if row == None:
        return None

    return row[0]
    
def edit_text(text_):
    "Edit text, using external editor"
    was_unicode = False
    if isinstance(text_, unicode):
        text = text_.encode(encoding)
        was_unicode = True
    else:
        text = text_
    
    #save the data to temp file
    try:
        fileno, path = tempfile.mkstemp(text=True)
        tfile = os.fdopen(fileno, "w")
        tfile.write(text)
        tfile.close()
    except IOError, e:
        print "Error creating temp file", e
        return None
    
    #run the editor
    rval = os.system("%s \"%s\""%(editor, path))
    
    if rval != 0:
        print "Call to editor failed, code", rval
        return None
    
    #read againthe temp file
    try:
        tfile = open(path, "r")
        new_text = tfile.read()
        tfile.close()
        os.remove(path)
    except IOError, e:
        print "Error reading temp file", e
        return None
    
    #convert if needed
    if was_unicode:
        return new_text.decode(encoding)
    else:
        return new_text
        

def describe_file(path):
    file_id=get_id(os.path.abspath(path))
    if file_id==None:
        print "File not found in the database"
        return
    desc = get_desc(file_id)
    #use the temp file
    if desc == None:
        desc = ""
    
    new_desc = edit_text(desc)
    if new_desc == None:
        print "Error editing file"
        return
        
    
    if new_desc == desc:
        print "Descriptiotn not changed"
        return
    else:
        print "Updating description..."
        if new_desc=="": new_desc = None
        set_desc(file_id, new_desc)        
    
        
    
    

####################################################################
if len(sys.argv) <=1:
    show_help()
    exit(0)
cmd = sys.argv[1]
####################################################################


create_tables()


if cmd == "add":
    fnames = sys.argv[2:]
    for fname in fnames:
        print "Adding file", fname
        add_file(fname, False)
    connection.commit()
    exit (0)

if cmd == "exists":
    fname = sys.argv[2]
    if check_present(os.path.abspath(fname)):
        print "File present in the DB"
        exit(0)
    else:
        print "File not present"
        exit(1)
        

if cmd == "show":
    fnames = sys.argv[2:]
    for fname in fnames:
        show_info(fname)
    exit (0)

if cmd == "describe":
    describe_file(sys.argv[2])


print "Closing connection"
cursor.close()
connection.close()
