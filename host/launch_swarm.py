import asyncio
import sys
import http.server
import socketserver
import socket
from websockets.server import serve
from threading import Thread

Ip_Dir = socket.gethostbyname(socket.gethostname())
HTTP_PORT = 8000
WEBSOCKET_PORT = 8080
clients = []

print("Server Ip Address : ", Ip_Dir)


class SimpleHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def log_message(self, format, *args):
        pass


def start_file_server():
    with socketserver.TCPServer(("", HTTP_PORT), SimpleHTTPRequestHandler) as httpd:
        print("File server started at port", HTTP_PORT)
        httpd.serve_forever()


async def echo(websocket):
    clients.append(websocket)
    async for message in websocket:
        print("Received message:", message)
        # if(message=="Give_Clients"):
        # await websocket.send("Number of clients : " + str(len(clients)))
        for client in clients:
            await client.send(message)


async def main():
    async with serve(echo, Ip_Dir, WEBSOCKET_PORT):
        print("Websocket server started at port", WEBSOCKET_PORT)
        try:
            await asyncio.Future()
        except asyncio.CancelledError:
            pass

if __name__ == '__main__':
    t = Thread(target=start_file_server, daemon=True).start()
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        sys.exit(0)
