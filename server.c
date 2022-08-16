/**
 * @file server.c
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

void error(const char message[]){
    perror(message);
    exit(1);
}
int main(int argc, char* argv[]){
    if(argc<2){
        error("PORT number is not provided!");
    }
    int sock_fd,newsock_fd,port_number,n;
    char buffer[255];
    struct sockaddr_in server_addr, cli_addr;
    socklen_t clie_len;
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0) error("Socket not opened!");
    bzero((char *) &server_addr,sizeof(server_addr));
    port_number = atoi(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);
    if(bind(sock_fd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0) error("Cannot able to find!");
    listen(sock_fd,5);
    clie_len = sizeof(cli_addr);
    newsock_fd = accept(sock_fd,(struct sockaddr *)&cli_addr,&clie_len);
    if(newsock_fd<0) error("Error on accept!");
    while(1){
        bzero(buffer,255);
        n = read(newsock_fd,buffer,255);
        if(n<0) error("Error on reading!");
        while(strlen(buffer)==0) n = read(newsock_fd,buffer,255);
        printf("\033[4;36mAmjed parvaiz:\033[0m %s\n",buffer);
        bzero(buffer,255);
        printf("\033[4;33mAbhishek kumar:\033[0m ");
        scanf("%[^\n]%*c", buffer);
        n = write(newsock_fd,buffer,strlen(buffer));
        if(n<0) error("Error on writing!");
        if(strcmp(buffer,"bye")==0) break;
    }
    close(newsock_fd);
    close(sock_fd);
    return 0; 
}


