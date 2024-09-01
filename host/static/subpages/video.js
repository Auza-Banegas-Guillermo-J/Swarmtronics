const startStreamButton = document.getElementById("start-stream");
const streamImageElement = document.querySelector(".stream");
const BASE_URL = "http://192.168.0.59" + ":81";
  startStreamButton.addEventListener("click", () => {
  streamImageElement.classList.remove("hidden");
  streamImageElement.src = BASE_URL + "/stream";
  });
