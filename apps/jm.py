import sqlite3 as sql

SQL="""
create table item( id integer primary key, name text, size integer, modified integer);
create table instance( id integer primary key, media_id integer, path text);
create table media(id integer primary key, default_mount_point text, mount_point text, is_removable integer, name text);
create table tags (id_item integer, id_tag integer, value text);
create table tag (id integer primary key, name text);
"""

database = ":memory:"

class JM:
    def __init__(self, db):
        if db:
            self.connect( db )

    def read_media(self):
        self.cursor.execute("select id, default_mount_point, mount_point, is_removable, name from media")
        for row in self.cursor:
            print row
            

    def add_media(self, name, mount_point, is_removable=True):
        self.cursor.execute("insert into media(name, default_mount_point, mount_point, is_removable) values (?,?,?,?)",
                            (name, mount_point, mount_point, is_removable ))
        media_id = self.cursor.lastrowid
        return media_id

    def connect(self, database):
        "Conenct to the database"
        self.connection = sql.connect( database )
        self.cursor = self.connection.cursor()
        self.check_database()
    
    def check_database(self ):
        "Ensure, that database has neede structure"
        self.cursor.execute("select (name) from sqlite_master where type='table' and name='item'")
        if self.cursor.fetchone() == None:
            #no databases
            print "Initializing database"
            self.connection.executescript( SQL )


    def get_tag(self, tag_name, create_new = False ):
        "Get tag ID and register new tag, if there is no such tag"
        self.cursor.execute("select (id) from tag where name=?", (tag_name,))
        row = self.cursor.fetchone()
        if row != None:#FIXME
            return row[0]
        else:
            #print "Tag %s not found"%tag_name
            if create_new:
                self.cursor.execute("insert into tag(name) values (?)", (tag_name,))
                return self.cursor.lastrowid
            else:
                return None #creation is forbidden - return None then

    def get_item(self, path):
        apath = os.path.abspath( path )
        if not os.path.exists( path ):
            raise ValueError, "File %s not found"%path
        #Detect media for this file, basin on its absolute path
        media = self.find_media( apath )
        


jm = JM( database )

print "Getting tag test twice"
print jm.get_tag("test", True)
print jm.get_tag("test")

jm.add_media( "root media", "/", False )
jm.read_media()
