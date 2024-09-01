const controlPanelElement = document.querySelector(".control-panel");
const websocketConnectButton = document.getElementById("websocket-connect");

const WEBSOCKET_PORT = 8080;
const WEBSOCKET_URI = "192.168.1.252";
const WEBSOCKET_URL = `ws://${WEBSOCKET_URI}:${WEBSOCKET_PORT}`;

const html = String.raw;

const nodeContainerTemplate = (nodeId) => html`
  <div class="node-container">
    <h2 style="text-align: center">Nodo ${nodeId + 1}:</h2>
    <div class="button-set">
      <button data-node="${nodeId}" class="arrow-button">&#8593;</button>
      <button data-node="${nodeId}" class="arrow-button">&#8592;</button>
      <button data-node="${nodeId}" class="arrow-button">&#8594;</button>
      <button data-node="${nodeId}" class="arrow-button">&#8595;</button>
    </div>
  </div>
`;

for (let i = 0; i < 5; i++) {
  controlPanelElement.innerHTML += nodeContainerTemplate(i);
}

const nodeDirections = {
  "↑": "w",
  "←": "a",
  "→": "d",
  "↓": "s",
};

let socket = null;

function startWebsocket() {
  socket = new WebSocket(WEBSOCKET_URL);

  socket.addEventListener("open", () => {
    console.log("Connected to Websocket:", WEBSOCKET_URL);
  });

  socket.addEventListener("error", () => {
    console.log("Error connecting to the socket");
  });
}

websocketConnectButton.addEventListener("click", startWebsocket);

controlPanelElement.addEventListener("click", (event) => {
  if (!event.target.classList.contains("arrow-button")) return;
  if (!socket) return;

  const nodeDirection = nodeDirections[event.target.innerText];
  const nodeId = event.target.dataset.node;

  socket.send(JSON.stringify({ nodeId, nodeDirection }));
});