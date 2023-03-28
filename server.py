import http.server
import shutil

class DeltaHTTPRequestHandler(http.server.BaseHTTPRequestHandler):
	def do_GET(self):
		if self.path=='/':
			self.send_response(http.server.HTTPStatus.OK)
			self.send_header("Content-type","text/html")
			self.end_headers()
			shutil.copyfileobj(open("index.html","rb"),self.wfile)
		else:
			self.send_error(http.server.HTTPStatus.NOT_FOUND)

if __name__=='__main__':
	server_address=('',8000)
	httpd=http.server.HTTPServer(server_address,DeltaHTTPRequestHandler)
	httpd.serve_forever()