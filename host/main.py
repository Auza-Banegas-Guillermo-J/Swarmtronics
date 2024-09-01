import os
from typing import List

from fastapi import FastAPI, WebSocket
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

app = FastAPI()

clients: List[WebSocket] = []
current_dir = os.path.dirname(os.path.abspath(__file__))
static_dir = os.path.join(current_dir, "static")


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    clients.append(websocket)

    while True:
        data = await websocket.receive_text()
        print("[+] RECEIVED MESSAGE", data)
        for client in clients:
            await client.send_text(data)


app.mount("/", StaticFiles(directory=static_dir, html=True), name="static")
