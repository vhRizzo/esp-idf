from os import curdir
from os.path import join as pjoin
from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl

class StoreHandler(BaseHTTPRequestHandler):
    store_path = pjoin(curdir, 'store.json')

    def do_GET(self):
        if self.path == '/store.json':
            with open(self.store_path) as fh:
                self.send_response(200)
                self.send_header('Content-type', 'text/json')
                self.end_headers()
                self.wfile.write(fh.read().encode())

    def do_POST(self):
        if self.path == '/store.json':
            length = self.headers['content-length']
            data = self.rfile.read(int(length))

            with open(self.store_path, 'w') as fh:
                fh.write(data.decode())

            self.send_response(200)

httpd = HTTPServer(('192.168.0.102', 8070), BaseHTTPRequestHandler)

httpd.socket = ssl.wrap_socket (httpd.socket, 
        keyfile="D:/Documents/VSCodes/school_monitor/server_certs/ca_key.pem", 
        certfile='D:/Documents/VSCodes/school_monitor/server_certs/ca_cert.pem', server_side=True)

httpd.serve_forever()