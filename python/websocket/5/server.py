from socket import * 
from threading import *
HOST = 'localhost'
PORT = 21567
BUFSIZ = 1024
ADDR = (HOST, PORT)
serversock = socket(AF_INET, SOCK_STREAM)
serversock.bind(ADDR)
serversock.listen(2)
while 1:
       print 'waiting for connection…'
       clientsock, addr = serversock.accept()
       print '…connected from:', addr
       thread.start_new_thread(handler, (clientsock,
addr))
 
serversock.close()