#!/usr/bin/env python
"""
LED Display Data server
"""

import BaseHTTPServer
import time
import sys
import led
import image
import multiboards


HOST_NAME = 'localhost'
PORT_NUMBER = 8080

class RedirectHandler(BaseHTTPServer.BaseHTTPRequestHandler):
   def do_HEAD(s):
      s.send_response(200)
      s.send_header("Content-type", "text/html")
      s.end_headers()

   def do_PUT(s):
      print "PUT path: " + s.path

      """Respond to a GET request."""
      self.send_response(200)
      self.send_header("Content-type", "text/xml")
      self.send_header("Content-length", str(len(response)))
      self.end_headers()
      self.wfile.write(response)

      # shut down the connection
      self.wfile.flush()
      self.connection.shutdown(1)



   def do_GET(s):
      """Respond to a GET request."""

      if s.path == "/data":
         s.send_response(200)
         s.send_header("Content-type", "text/plain")
         s.end_headers()
         s.wfile.write("bcd\ndefg")
      else:
         s.send_response(200)
         s.send_header("Content-type", "text/html")
         s.end_headers()

         s.wfile.write("<html><head><title>Title goes here.</title></head>")
         s.wfile.write("<body><p>This is a test.</p>")
         # If someone went to "http://something.somewhere.net/foo/bar/",
         # then s.path equals "/foo/bar/".
         s.wfile.write("<p>You accessed path: %s</p>" % s.path)
         s.wfile.write("</body></html>")


   def is_rpc_path_valid(self):
       return self.path == '/image'
           
        
   def do_POST(self):
        """Handles the HTTP POST request.

        Attempts to interpret all HTTP POST requests as XML-RPC calls,
        which are forwarded to the server's _dispatch method for handling.
        """

        # Check that the path is legal
        if not self.is_rpc_path_valid():
            self.report_404()
            return

        try:
            # Get arguments by reading body of request.
            # We read this in chunks to avoid straining
            # socket.read(); around the 10 or 15Mb mark, some platforms
            # begin to have problems (bug #792570).
            max_chunk_size = 10*1024*1024
            size_remaining = int(self.headers["content-length"])
            L = []
            while size_remaining:
                chunk_size = min(size_remaining, max_chunk_size)
                L.append(self.rfile.read(chunk_size))
                size_remaining -= len(L[-1])
            data = ''.join(L)

            """
            # In previous versions of SimpleXMLRPCServer, _dispatch
            # could be overridden in this class, instead of in
            # SimpleXMLRPCDispatcher. To maintain backwards compatibility,
            # check to see if a subclass implements _dispatch and dispatch
            # using that method if present.
            response = self.server._marshaled_dispatch(
                    data, getattr(self, '_dispatch', None)
                )
            """
        except Exception as e: # This should only happen if the module is buggy
            print 'Exception: ', e
            # internal error, report as HTTP server error
            self.send_response(500)
            self.end_headers()
        else:
            response = ''
            # got a valid XML RPC response
            self.send_response(200)
            self.send_header("Content-type", "text/xml")
            self.send_header("Content-length", str(len(response)))
            self.end_headers()
            self.wfile.write(response)

            # shut down the connection
            self.wfile.flush()
            self.connection.shutdown(1)

            # process data
            print len(data) #, list(data)
            image.image_buffer[image.image_current % image.IMAGE_BUFFER_LENGTH] = data
            image.image_current += 1

   def report_404 (self):
            # Report a 404 error
        self.send_response(404)
        response = 'No such page'
        self.send_header("Content-type", "text/plain")
        self.send_header("Content-length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)
        # shut down the connection
        self.wfile.flush()
        self.connection.shutdown(1)




if __name__ == '__main__':
   server_class = BaseHTTPServer.HTTPServer
   httpd = server_class((HOST_NAME, PORT_NUMBER), RedirectHandler)
   print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
   try:
      httpd.serve_forever()
   except KeyboardInterrupt:
      pass
   httpd.server_close()
   print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)

