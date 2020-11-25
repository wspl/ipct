const net = require('net')

net.createServer(socket => {
    socket.on('data', data => {
        console.log(data)
        socket.write(data)
    })
    socket.on('error', e => console.error(e))
}).on('error', e => console.error(e))
    .listen(12345)