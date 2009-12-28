#!/usr/bin/python

import sys
import sqlite3 as sql
import getopt
import os
import tempfile

#path to the database file
db_path = os.path.join(os.environ["HOME"], ".junkmaster.db") 
connection = sql.connect(db_path)
cursor = connection.cursor()

editor = "vi"
encoding = "utf-8"

def create_tables():
    print "Creating tables"
    #table for storing basic file data
    cursor.execute("create table if not exists JUNK (id INTEGER PRIMARY KEY, name TEXT, folder TEXT, size INTEGER, hash TEXT)")
    #table for file descriptions
    cursor.execute("create table if not exists DESC (id INTEGER UNIQUE, desc TEXT)")
    #table for variable parameters
    cursor.execute("create table if not exists FIELDS (idfile INTEGER, name TEXT, value TEXT)")
    #table for tags
    cursor.execute("create table if not exists TAGS (idfile INTEGER, name TEXT)")
    
    connection.commit()
    print "Done"
    

def add_file(fname, commit = True):
    "Add file to the database"
    abs_path = os.path.abspath(fname)
    
    if check_present(abs_path): #file is already added
        print "File %s is already added."%fname
        return False
    
    folder,name = os.path.split(abs_path)
    size = os.path.getsize(abs_path)
    
    cursor.execute("insert into JUNK (id, name, folder, size) values(NULL, ?, ?, ?)", (name, folder, size))
    
    if commit:
        connection.commit()



def check_present(apath):
    folder, name = os.path.split(apath)
    
    #exact match
    rows = cursor.execute("select id, size from JUNK where name=? and folder=?",(name, folder))
    for id, sz in rows:
        print "File already exists: %s"%apath
        return True
    return False
    
def show_help():
    print "junkmaster command ...."


def show_info(fname):
    apath = os.path.abspath(fname)
    folder, name = os.path.split(apath)
    
    rows = list(cursor.execute("select id, size from JUNK where name=? and folder=?", (name, folder)))
    
    if len(rows) == 0:
        print "File not in database:", apath
        
    if len(rows)>1:
        print "Multiple file records!"
        
    
def get_tags(file_id):
    "Returns list of tags"
    rows = cursor.execute("select name from TAGS where idfile=?",(file_id,))
    return [row[0] for row in rows]

def add_tags(file_id, taglist, commit=True):
    for tag in taglist:
        cursor.execute("insert or ignore into TAGS (idfile, name) VALUES (?, ?)",(file_id, tag))
    if commit:
        connection.commit()

def remove_tags(file_id, taglist, commit = True):
    "Remove tags from file"
    for tag in taglist:
        cursor.execute("delete or ignore from TAGS where idfile=? and name=?",(file_id, tag))
    if commit:
        connection.commit()

def get_desc(file_id):
    "Return the description"
    row = cursor.execute("select desc from DESC where id=?", (file_id,)).fetchone()
    if row == None:#nothing fetched
        return None
    return row[0]
    
def set_desc(file_id, desc, commit=True):
    "Set the file description"
    if desc != None:
        print "Setting description..."
        cursor.execute("insert or replace into DESC (id, desc) values(?, ?)",(file_id, desc))
    else:
        print "Deleting descrtiption..."
        cursor.execute("delete from DESC where id=?",(file_id,))
        
    if commit:
        connection.commit()

    

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


print "testing"
create_tables()


if cmd == "add":
    fnames = sys.argv[2:]
    for fname in fnames:
        print "Adding file", fname
        add_file(fname, False)
    connection.commit()
    exit (0)

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
