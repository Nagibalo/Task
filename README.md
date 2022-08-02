# TCP-Client-Server
Simple multithread server based on TCP-protocol and client
Server includes echo-service and file-service. Communication with the server occurs through commands

Target platform: Linux

Using technology:
1) Sockets based on sys/socket.h linux 
2) Pool of thread based on thread.h c++11
3) Thread synchronization with mutexes and atomic values (mutex.h, atomic.h c++11)
4) Unix-signal processing with handler (signal.h)
5) Smart pointer: unique_ptr, shared_ptr (memory.h c++11)

# Classes
1) TCPSocket - socket class includes struct sockaddr_in, port and perform all action with socket (connect, bind, shutdown and much more)
2) TCPServer - server class includes TCPSocket. Main fucntion:
* Сreate server socket 
* Run loop accept connection in threading pool
* Run client hadler in threading pool 
* Run loop signal handler in main thread
* Correct shutdown server/clients socket and terminate thread
3) TCPClient - client class includes TCPSocket. Main fucntion:
* Create client socket
* Communicate with server
4) ThreadPool - thread pool class includes vector of threads, mutex condition and queue of tasks. Main fucntion:
* Create threads with task wait loop
* Add task in queue
* Correct terminate all threads

# Work server
1. Run
<div id="header" align="center">
  <img src="https://github.com/VivanDenon/Images/blob/main/TCPServer/TCPServer.PNG"/>
</div>

2. Closed
<div id="header">
  <img src="https://github.com/VivanDenon/Images/blob/main/TCPServer/TCPServerClosed.PNG"/>
</div>

# Work client
<div id="header" align="center">
  <img src="https://github.com/VivanDenon/Images/blob/main/TCPServer/TCPClient.PNG"/>
</div>
