import http.cookies
import http.server
import shutil

class DeltaCookie(http.cookies.BaseCookie):
	def value_decode(self,val):
		return val,val
	def value_encode(self,val):
		strval=str(val)
		return strval,strval

class DeltaHTTPRequestHandler(http.server.BaseHTTPRequestHandler):
	def do_GET(self):
		if self.path=='/favicon.ico':
			f=open(self.path[1:],"rb")
			self.send_response(http.server.HTTPStatus.OK)
			self.send_header("Content-type","image/x-icon")
		elif self.path[:8]=='/assets/':
			try:
				f=open(self.path[1:],"rb")
				self.send_response(http.server.HTTPStatus.OK)
				if self.path[-4:]=='.css':
					self.send_header("Content-type","text/css")
				elif self.path[-3:]=='.js':
					self.send_header("Content-type","application/x-javascript")
				elif self.path[-4:]=='.map' or self.path[-5:]=='.json':
					self.send_header("Content-type","application/json")
			except IOError:
				self.send_error(http.server.HTTPStatus.NOT_FOUND)
				return
		elif self.path=='/':
			f=open("index.html","rb")
			self.send_response(http.server.HTTPStatus.OK)
			self.send_header("Content-type","text/html;char-set=utf-8")
			self.send_header("Set-Cookie","Delta=test;Max-age=604800")
		else:
			self.send_error(http.server.HTTPStatus.FORBIDDEN)
			return
		self.end_headers()
		shutil.copyfileobj(f,self.wfile)

if __name__=='__main__':
	server_address=('',8000)
	httpd=http.server.HTTPServer(server_address,DeltaHTTPRequestHandler)
	httpd.serve_forever()