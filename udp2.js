const dgram = require('dgram');
const server = dgram.createSocket('udp4');


server.on('message', (msg, rinfo) => {
    console.timeEnd(msg.toString())
    send()
});

server.bind(41235);

function send() {
    const rd = `${Math.random()}`
    console.time(rd)
    server.send(rd, 41234)
}

send()