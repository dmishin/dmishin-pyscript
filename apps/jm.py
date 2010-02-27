import sqlite3 as sql

SQL="""
create table item( id integer primary key, name, size, modified);
create table instance( id integer primary key, media_id, path);
create table media(id integer primary key, default_mount_point, mount_point, is_removable, name);
create table tags (id_item, id_tag, value);
create table tag (id integer primary key, name);
"""

database = ":memory:"

class JM:
    def __init__(self, db):
        if db:
            self.connect( db )

    def connect(self, database):
        "Conenct to the database"
        self.connection = sql.connect( database )
        self.cursor = self.connection.cursor()

        self.check_database()
    
    def check_database(self ):
        "Ensure, that database has neede structure"
        self.cursor.execute("select (name) from sqlite_master where type='table' and name='item'")
        if not self.cursor == 0:#FIXME
            #no databases
            print "Initializing database"
            self.connection.executescript( SQL )


    def get_tag(self, tag_name, create_new = False ):
        "Get tag ID and register new tag, if there is no such tag"
        self.cursor.execute("select (id) from tag where name=?", (tag_name,))
        if not self.cursor == 0:#FIXME
            print "Tag %s not found"%tag_name
            self.cursor.execute("insert into tag(name) values (?)", (tag_name,))
            return self.cursor.lastrowid
        return list(self.cursor)
        pass

    def get_item(self, path):
        apath = os.path.abspath( path )
        if not os.path.exists( path ):
            raise ValueError, "File %s not found"%path
        #find media
        media = self.find_media( apath )


jm = JM( database )

print "Getting tag test twice"
print jm.get_tag("test")
print jm.get_tag("test")
