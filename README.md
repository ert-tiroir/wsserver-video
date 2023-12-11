
# wsvideo

wsvideo is an extension of the web socket server wsserver designed to take as input a video flux and to pass it to a video player in the browser through web socket by broadcasting it to all the known clients.

# wsvideo Broadcaster

The broadcaster for the video service is made in C++ and works with the [wsserver](https://github.com/ert-tiroir/wsserver) library that handles the background websocket. You can create a broadcaster for a server with the flux name in the following way :

```c++
VideoBroadcaster broadcaster = new VideoBroadcaster(server, flux_name);
```

Once you have done this, you can send the video data as a string using the sendPacket method. The video data should be a binary flux coming either from a video file or a video generation tool like ffmpeg to send the necessary data through the flux. Whenever you call something like server.listen() to get a new client or you create a new packet, don't forget to use the tick method, so that new clients start receiving packets.

You can find a full example in example.cpp

# wsvideo Player

With the broadcaster comes a javascript file that you have to include in your project for the video to play. You can easily create a video viewer from the web socket and the flux name in the following way :

```js
let wsvideo = new WSVideo(websocket, flux_name, codec)
```

For the codec, you can hardcode it if you know in advance what your video codec will be. Otherwise, you could try using a library server-side to compute the codec and send it via web socket to your client.

You can then attach your video element by using a query selector or something like that :

```js
wsvideo.attach( document.querySelector("video") )
```