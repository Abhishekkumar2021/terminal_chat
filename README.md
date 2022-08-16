# 5 Must know fundamentals of socket programming in c
## 1. Socket
### Creating a socket in c : 
```c
#include<sys/socket.h>
int socket(int domain,int type,int protocol);
```

## 2. Bind
### Binding the socket
#### We have to bind the socket to a particular ***PORT*** on our machine to start transferring data.
```c
#include<sys/socket.h>
int bind(int sockfd,struct sockaddr *my_address, socklen_t addrlen);
```
where,
#### ***sockfd*** is - socket file descriptor
#### ***sockaddr*** is - socket address
#### ***socklen_t*** is - socket address length

## 3. Listen
### Listenning for conection request
```c
#include<sys/socket.h>
int listen(int sockfd,int backlog);
```
## 4. Accept
### Accepting the incoming socket conection request
```c
#include<sys/socket.h>
int listen(int sockfd,struct sockaddr *my_address, socklen_t addrlen);
```
## 5. Connect
### Attempting to establish a connection
```c
#include<sys/socket.h>
int connect(int sockfd,struct sockaddr *server_address, socklen_t addrlen);
```

