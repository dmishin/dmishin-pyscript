import sqlite3 as sql


class JM:

    def __init__(self, db):
        self.db = db
        self.connection = sql.connect(db)
        self.cursor = connection.cursor()


    def new_item(self, name):

        self.cursor.execute(\
            "insert into Items (name) values (?)", name)

    def new_file(self, item, 
