# TCP-Client-Server
Simple multithread server based on TCP-protocol and client

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
  
  a) Сreate server socket 
  
  b) Run loop accept connection in threading pool
  
  c) Run client hadler in threading pool 
  
  d) Run loop signal handler in main thread
  
  e) Correct shutdown server/clients socket and terminate thread
3)  TCPClient - client class includes TCPSocket. Main fucntion:
  
  a) Create client socket
  
  b) Communicate with server
