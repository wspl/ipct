const dgram = require('dgram');
const server = dgram.createSocket('udp4');


server.on('message', (msg, rinfo) => {
    server.send(msg, 41235)
});

server.bind(41234);