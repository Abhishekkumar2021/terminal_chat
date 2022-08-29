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
    // for(int i=0; i<idx; i++){
    //     printf("%d",arr[i]);
    // }

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
    // for(int i=0; i<idx; i++){
    //     printf("%d",arr[i]);
    // }

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
    if(argc<2){
        error("PORT number is not provided!");
    }
    int sock_fd,newsock_fd,port_number,n;
    char buffer[MAX_LEN],name[MAX_LEN];
    int code = 2; //read initialise
    struct sockaddr_in server_addr, cli_addr;
    socklen_t clie_len;
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0) error("\033[4;31mSocket not opened!\033");
    printf("\033[4;32mSocket created succesfully.\033[0m\n");
    bzero((char *) &server_addr,sizeof(server_addr));
    port_number = atoi(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);
    if(bind(sock_fd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0) error("Cannot able to bind!");
    printf("\033[4;32mBinded succesfully to PORT %d.\033[0m\n",port_number);
    listen(sock_fd,10);
    printf("\033[4;32mListenning on PORT %d.\033[0m\n",port_number);
    printf("*********************************************\n");
    printf("*        How this message app works ?       *\n");
    printf("*        Firstly you have to type a code    *\n");
    printf("*        Firstly you have to type a code    *\n");
    printf("*        Type 1 and then message to send    *\n");
    printf("*        Type other than 1 to to exit app.        *\n");
    printf("*********************************************\n");
    clie_len = sizeof(cli_addr);
    newsock_fd = accept(sock_fd,(struct sockaddr *)&cli_addr,&clie_len);
    if(newsock_fd<0) error("Error on accept!");
    n = read(newsock_fd,name,MAX_LEN);
    if(n<0) error("Error on reading!");
    printf("\033[4;33m%s \033\033[4;32mis connected successfully.\033\n",name);
    while(1){
        char ch[1];
        ch[0]='\0';
        scanf("%d",&code);
        read:
        if(code==2){
            n = read(newsock_fd,ch,1);
            if(n<0) error("Error on reading!");
            bzero(buffer,MAX_LEN);
            n = read(newsock_fd,buffer,MAX_LEN);
            while(strlen(buffer)==0) n = read(newsock_fd,buffer,MAX_LEN);
            if(n<0) error("Error on reading!");
            decode(buffer);
            printf("\033[4;36m%s:\033[0m %s\n",name,buffer);
            bzero(buffer,MAX_LEN);
            if(ch[0]=='2'){
                code = 2;
                goto read;
            }
            scanf("%d",&code);
            if(code!=1){
                close(newsock_fd);
                exit(0);
            }else{
                goto write;
            }
        }
        write:
        if(code==1){
            printf("\033[4;33mAbhishek kumar:\033[0m ");
            scanf("%[^\n]%*c", buffer);
            encode(buffer);
            n = write(newsock_fd,buffer,strlen(buffer));
            if(n<0) error("Error on writing!");
            if(strcmp(buffer,"bye")==0){
                close(newsock_fd);
                exit(0);
            }
            scanf("%d",&code);
            if(code!=1){
                close(newsock_fd);
                exit(0);
            }else{
                goto write;
            }
        }
        
    }
    close(newsock_fd);
    close(sock_fd);
    return 0; 
}


