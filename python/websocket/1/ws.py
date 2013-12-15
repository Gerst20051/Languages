import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(("", 9999))
sock.listen(5)

handshake = '\
HTTP/1.1 101 Web Socket Protocol Handshake\r\n\
Upgrade: WebSocket\r\n\
Connection: Upgrade\r\n\
WebSocket-Origin: http://localhost:8888\r\n\
WebSocket-Location: ws://localhost:9999/\r\n\r\n\
'
handshaken = False

print "TCPServer Waiting for client on port 9999"

import sys

data = ''
header = ''

client, address = sock.accept()
while True:
    if handshaken == False:
        header += client.recv(16)
        if header.find('\r\n\r\n') != -1:
            data = header.split('\r\n\r\n', 1)[1]
            handshaken = True
            client.send(handshake)
    else:
            tmp = client.recv(128)
            data += tmp;

            validated = []

            msgs = data.split('\xff')
            data = msgs.pop()

            for msg in msgs:
                if msg[0] == '\x00':
                    validated.append(msg[1:])

            for v in validated:
                print v
                client.send('\x00' + v + '\xff')