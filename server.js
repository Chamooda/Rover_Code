var WebSocketServer= require('websocket').server;
var http = require('http');
const net = require('net');
const port = 8083;
let ESP_01="";


var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});
server.listen(8082, function() {
    console.log((new Date()) + ' Server is listening on port 8082');
});

const wsServer = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

let connections={}
let ESPIP="192"
let ClientIP=""
Update=""
let client1=null
let client2=null
wsServer.on('request', (socket)=>{
    var connection = socket.accept(null, socket.origin)
    console.log(socket)
    if (!client1) {
        client1 = connection;
    } else if (!client2) {
        client2 = connection;
    }

        
    console.log((new Date()) + ' Connection accepted.');
    // connection.on('connected',function(message)
    // {
    //     if (ESP_01!="")
    //     {
    //         connection.send(ESP_01)
    //     }
    // })
    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            
            if (message.utf8Data.slice(0,3)=="ESP")
            {
                
                ESPIP=message.utf8Data.slice(4)
                console.log("ESP IP Address: ",ESPIP)
                
            }
            else if(message.utf8Data.slice(0,6)=="client")
            {
                Update=message.utf8Data.slice(6)              
                
            }
            
            console.log('Received Message: ' + message.utf8Data);
            //connection.sendUTF(message.utf8Data); //this resend the reseived message, instead of it i will send a custom message. hello from nodejs
            
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            connection.sendBytes(message.binaryData);
        }
        if (connection === client1 && client2) {
            // Forward message from Client1 to Client2
            client2.sendUTF(message.utf8Data);
            
        } else if (connection === client2 && client1) {
            // Forward message from Client2 to Client1
            client1.sendUTF(message.utf8Data);
        }

    });



    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
        if (connection === client1) {
            client1=null;            
        } 
        else if (connection === client2) {
            client2=null;        
        }
    });
});


// For Arduino :) 


// const server2 = net.createServer((socket) => {
//     console.log('Client connected.');
  
//     socket.on('data', (data) => {
//       const receivedData = data.toString();
//       ESP_01=receivedData;
//       console.log(`Received data: ${receivedData}`);
//       // Here, you can process the received data as needed.
  
//       // Example: Send a response back to the client (ESP-01).
//       const response = "Server received: " + receivedData;
//       socket.write(response);
//     });
  
//     socket.on('end', () => {
//       console.log('Client disconnected.');
//     });
  
//     socket.on('error', (err) => {
//       console.error(`Error: ${err.message}`);
//     });
//   });
  
//   server2.listen(port, () => {
//     console.log(`Server listening on port ${port}`);
//   });
  
