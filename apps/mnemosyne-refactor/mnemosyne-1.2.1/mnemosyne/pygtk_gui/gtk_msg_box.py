from gtk_import import *


def ShowMsgBox( owner, title, message, buttons = (gtk.STOCK_CLOSE, gtk.RESPONSE_CLOSE)):
    
    dia = gtk.Dialog(title,
                  owner,
                  gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,  #binary flags or'ed together
                  buttons)
    dia.vbox.pack_start(gtk.Label( message ))
    dia.show_all()
    result = dia.run()
    dia.destroy()
    return result

if __name__ == '__main__':
    print "Testing dialog box"
    print "returned value:", ShowMsgBox( None, "test", "This is the message text to ask", (gtk.STOCK_OK, gtk.RESPONSE_OK)  )
