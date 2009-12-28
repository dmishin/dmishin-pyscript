import sqlite3 as sql

HELP_MESSAGE = """
Type the SQL operator to execute it

Special commands:
  OPEN datafile
  CLOSE
  TABLES
  HELP
"""


class REP:
    def __init__(self):
        self.connection = None
        self.cursor = None
        self.special_commands ={
            "OPEN": self.cmd_open,
            "CLOSE": self.cmd_close,
            "TABLES": self.cmd_tables,
	    "HELP": self.cmd_help,
            };
    def cmd_help(self, args):
        print HELP_MESSAGE

    def cmd_tables(self, args):
        if len (args ) != 0:
            print "Error: use TABLES"
            return
        self.cursor.execute("select (name) from sqlite_master where type='table'")
        for row in self.cursor:
            print u"[%s]"%row[0],
        print 
        
    def cmd_open(self, args):
        if len(args) != 1:
            print "Error: specify file for OPEN"
            return
        
        if self.connection:
            self.cmd_close()
            
        print "Connecting to file", args[0]
        
        self.connection = sql.connect(args[0])
        self.cursor = self.connection.cursor()
        
    def cmd_close(self, args):
        if not self.connection:
            print "Nothing to CLOSE"
            return
            
        if len(args) != 0:
            print "Warning: garbage after CLOSE"
            
        self.cursor = None
        self.connection.close()
        self.connection = None


    def exec_special(self, command):
        cmds = command.split()
        try:
            self.special_commands[cmds[0].upper() ](cmds[1:])
        except Exception, msg:
            print "Error executing special command:", msg

    def run(self):    
        print "Type \"help\" to get help"
        while True:
            command = raw_input("SQL>");
            command = command.strip()
            
            if not command: continue
            
            cmd = command.split()[0].upper()
                
            if "QUIT".find(cmd)==0:#quit
               break

            if cmd in self.special_commands:
                self.exec_special(command)
            else:
                self.exec_sql(command)
                
    def exec_sql(self, command):
    #run sql
        if not self.cursor:
            print "Can not execute command, because there is no connection"
            return
        try:
            self.cursor.execute( command )
            self.connection.commit()
            #read
            for row in self.cursor:
                print row
                
        except Exception, msg:
            print "Exception:", msg
            
            
rep = REP()
rep.run()
