from struct import *
import threading
import hashlib
import string
import socket
import time
import re

class WebSocketThread(threading.Thread):

	def __init__ ( self, channel, details, websocket ):
		self.channel = channel
		self.details = details
		self.websocket = websocket
		threading.Thread.__init__ ( self )

	def run ( self ):
		print ("Monty> Received connection ", self.details [ 0 ])
		self.handshake(self.channel)
		while True:
			self.interact(self.channel)

	def finduser(self, client):
		for user in self.websocket.users:
			if user.socket == client:
				return user
		return 0

	def send_data(self, client, str):
		str = b"\x00" + str.encode('utf-8') + b"\xff"
		try:
			return client.send(str)
		except (IOError, e):
			if e.errno == 32:
				user = self.finduser(client)
				print ("Monty> pipe error")

	def recv_data(self, client, count):
		data = client.recv(count)
		return data.decode('utf-8', 'ignore')

	def recv_data_unencoded(self, client, count):
		data = client.recv(count)
		return data

	def get_headers(self, data):

		bytes = data[len(data)-8:]

		data = data.decode('utf-8', 'ignore')
		resource = re.compile("GET (.*) HTTP").findall(data)
		host = re.compile("Host: (.*)\r\n").findall(data)
		origin = re.compile("Origin: (.*)\r\n").findall(data)
		key1 = re.compile("Sec-WebSocket-Key1: (.*)\r\n").findall(data)
		key2 = re.compile("Sec-WebSocket-Key2: (.*)\r\n").findall(data)
		return [resource[0],host[0],origin[0],key1[0],key2[0],bytes]

	def part(self, token):
		digits=""
		for d in re.compile('[0-9]').findall(token):
			digits = digits + str(d)
		count=0
		for s in re.compile(' ').findall(token):
			count = count + 1
		return int(int(digits)/count)

	def handshake(self, client):
		shake = self.recv_data_unencoded(client, 255)
		headers = self.get_headers(shake)
		challenge = pack('>II8B', self.part(headers[3]), self.part(headers[4]), headers[5][0], headers[5][1], headers[5][2], headers[5][3], headers[5][4], headers[5][5], headers[5][6], headers[5][7])
		hash = hashlib.md5(challenge).digest()
		our_handshake = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"+"Upgrade: WebSocket\r\n"+"Connection: Upgrade\r\n"+"Sec-WebSocket-Origin: "+headers[2]+"\r\n"+"Sec-WebSocket-Location: "+" ws://"+headers[1]+headers[0]+"\r\n\r\n"
		client.send(our_handshake.encode('latin-1') + hash)

	def interact(self, client):
		users = self.websocket.users
		this_user = self.finduser(client)
		data = self.recv_data(client, 255)
		print (data)
		if(data[1:]=="CONNECTED"):
			self.send_data(this_user.socket, "Welcome")
		if(data[1:]=="POKE"):
			self.send_data(this_user.socket, "Quit poking me!")
