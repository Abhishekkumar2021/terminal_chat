/**
 * @file client.c
 * @author Abhishek kumar
 * @brief 
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char message[]){
    perror(message);
    exit(1);
}
int main(int argc, char* argv[]){
    int sock_fd,port_number,n;
    struct sockaddr_in server_addr;
    char buffer[255];
    if(argc<3){
        error("Provide two things\n1. Ip address of server.\n2. PORT number of server.\n");
    }
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0) error("Socket not opened!");
    port_number = atoi(argv[2]);
    struct hostent* server = gethostbyname(argv[1]);
    if(server==NULL) error("No such host found!");
    bzero((char *)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,(char *)&server_addr.sin_addr.s_addr,server->h_length);
    server_addr.sin_port = htons(port_number);
    if(connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0) error("Connection Failed !");
    while(1){
        printf("\033[4;33mAmjed parvaiz:\033[0m ");
        bzero(buffer,255);
        scanf("%[^\n]%*c", buffer);
        n = write(sock_fd,buffer,sizeof(buffer));
        if(n<0) error("Error writing!");
        bzero(buffer,255);
        if(n<0) error("Error on reading!");
        while(strlen(buffer)==0) n = read(sock_fd,buffer,255);
        printf("\033[4;36mAbhishek kumar:\033[0m %s\n",buffer);
        if(n<0) error("Error on writing!");
        if(strcmp(buffer,"bye")==0) break;
    }
    close(sock_fd);
    return 0; 
}


