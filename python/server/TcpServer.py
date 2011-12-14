#!/usr/bin/env python

import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
	def handle(self):
		while(True):
			print "request from {}:".format(self.client_address[0])

			typeId = int(self.recPart())
			seqId = int(self.recPart())
			compressed = int(self.recPart())
			print 'typeId = ', typeId, ' sequence= ', seqId, ' compressed=', compressed

			if typeId == 1: # image
				dataLen = int(self.recPart())
				image = self.recByte(dataLen)

				print "recived image ", dataLen

				self.request.send('OK\n')

			else:
				self.request.send('ERR\n')
				raise Exception('Paket with unknown type ID recived: %i' % typeId)

	def recByte(self, size):
		data = bytearray(size)
		for i in range(size):
			data[i] = self.request.recv(1)

		return data;


	def recPart(self):
		s = ''
		data = ''
		while data != ' ':
			s += data
			data = self.request.recv(1)
		
		return s.strip()

if __name__ == "__main__":
	HOST, PORT = "localhost", 28900

	server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

	server.serve_forever()

