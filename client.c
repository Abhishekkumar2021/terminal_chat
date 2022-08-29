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
#include<signal.h>
#define MAX_LEN 1000

int charToBinary(char c, int arr[], int idx){
    int x = c;
    for(int i=7; i>=0; i--){
        int bit  = (x>>i) & 1;
        arr[idx] = bit;
        idx++;  
    }
    return idx;
}
void fillArray(char ans[],int num,int c_idx){
    if(num<26) ans[c_idx] = 65 + num;
    else if(num<52) ans[c_idx] = (num - 26) + 97;
    else if(num<62) ans[c_idx] = (num - 52) + 48;
    else if(num==62) ans[c_idx] = '+';
    else ans[c_idx] = '/';
}
int power(int x,int y){
    int ans = 1;
    while(y>0){
        ans *= x;
        y--;
    }
    return ans;
}
void encode(char str[]){
    char ans[MAX_LEN];
    int idx = 0;
    int arr[MAX_LEN];
    for(int i=0; str[i]!='\0'; i++){
        idx = charToBinary(str[i],arr,idx);
    }

    int c_idx = 0;
    int cnt = 5;
    int num = 0;
    for(int i=0; i<idx; i++){
        num += arr[i]*power(2,cnt);
        cnt--;
        if(cnt<0){
            fillArray(ans,num,c_idx);
            c_idx++;
            cnt = 5;
            num = 0;
        }
    }
    if(idx%6){
        fillArray(ans,num,c_idx);
        c_idx++;
    }
    bzero(str,MAX_LEN);
    for(int i=0; i<c_idx; i++){
        str[i] = ans[i];
    }

}

void decode(char str[]){
    char ans[MAX_LEN];
    int idx = 0;
    int arr[MAX_LEN];
    int number = 0;
    for(int i=0; str[i]!='\0'; i++){
        if(str[i]>='A' && str[i]<='Z') number = str[i] - 65;
        else if(str[i]>='a' && str[i]<='z') number = str[i] - 71;
        else if(str[i]>='0' && str[i]<='9') number = str[i] + 4;
        else if(str[i]=='+') number = 62;
        else number = 63;
        int i = 5;
        while(i>=0){
            int bit  = (number>>i) & 1;
            arr[idx] = bit;
            idx++;
            i--;
        }
    }

    int c_idx = 0;
    int cnt = 7;
    int num = 0;
    for(int i=0; i<idx; i++){
        num += arr[i]*power(2,cnt);
        cnt--;
        if(cnt<0){
            ans[c_idx] = num;
            c_idx++;
            cnt = 7;
            num = 0;
        }
    }
    if(idx%8){
        ans[c_idx] = num;
        c_idx++;
    }
    bzero(str,MAX_LEN);
    for(int i=0; i<c_idx; i++){
        str[i] = ans[i];
    }

}

void error(const char message[]){
    perror(message);
    exit(1);
}
int main(int argc, char* argv[]){
    int sock_fd,port_number,n;
    struct sockaddr_in server_addr;
    char buffer[MAX_LEN], name[MAX_LEN];
    int code = 1; //write initialise
    if(argc<3){
        error("\033[4;31mProvide two things\n1. Ip address of server.\n2. PORT number of server.\033[0m\n");
    }
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0) error("\033[4;31mSocket not opened!\033[m");
    printf("\033[4;35mEnter your name please: \033[0m");
    scanf("%[^\n]%*c", name);
    port_number = atoi(argv[2]);
    struct hostent* server = gethostbyname(argv[1]);
    if(server==NULL) error("\033[4;31mNo such host found!\033");
    bzero((char *)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,(char *)&server_addr.sin_addr.s_addr,server->h_length);
    server_addr.sin_port = htons(port_number);
    printf("\033[4;32mConnected to server.\033[0m\n");
    printf("*********************************************\n");
    printf("*        How this message app works ?       *\n");
    printf("*        Firstly you have to type a code    *\n");
    printf("*        Firstly you have to type a code    *\n");
    printf("*        Type 1 and then message to send    *\n");
    printf("*        Type 2 to recieve a message        *\n");
    printf("*********************************************\n");
    if(connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0) error("\033[4;31mConnection Failed !\033");
    n = write(sock_fd,name,sizeof(name));
    if(n<0) error("\033[4;31mError writing!\033");
    while(1){
        char ch[1];
        scanf("%d",&code);
        write:
        if(code == 1){
            printf("\033[4;33m%s:\033[0m ",name);
            bzero(buffer,MAX_LEN);
            scanf("%[^\n]%*c", buffer);
            ch[0] = '1';
            n = write(sock_fd,ch,1);
            if(n<0) error("Error writing!");
            encode(buffer);
            n = write(sock_fd,buffer,sizeof(buffer));
            if(n<0) error("Error writing!");
            bzero(buffer,MAX_LEN);
            scanf("%d",&code);
            if(code!=1){
                close(sock_fd);
                exit(0);
            }else{
                goto write;
            }
        }
        read:
        if(code == 2){
            n = read(sock_fd,ch,1);
            if(n<0) error("Error on reading!");
            if(ch[0]=='1'){
                code = 1;
                goto write;
            }
            while(strlen(buffer)==0) n = read(sock_fd,buffer,MAX_LEN);
            if(n<0) error("Error on reading!");
            decode(buffer);
            printf("\033[4;36mAbhishek kumar:\033[0m %s\n",buffer);
            if(strcmp(buffer,"bye")==0){
                close(sock_fd);
                exit(0);
            }
            scanf("%d",&code);
            if(code==2){
                ch[0] = '\0';
                goto read;
            }else{
                goto write;
            }
        }
    }
    
    return 0; 
}


