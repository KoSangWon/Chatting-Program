//
//  main.c
//  client
//
//  Created by 고상원 on 2020/04/21.
//  Copyright © 2020 고상원. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define BUFF_SIZE 1024

int main(int argc, const char * argv[]) {
    int client_socket;

    struct ifreq ifr;
    
    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE];
    char message[BUFF_SIZE];
    char totalMessage[BUFF_SIZE];
    
    char ip_buf[BUFF_SIZE];

    
    while(1){

        client_socket = socket(PF_INET, SOCK_STREAM, 0);
        strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ);
        ioctl(client_socket, SIOCGIFADDR, &ifr);
        
        if(-1 == client_socket){
            printf("socket 생성 실패\n");
            exit(1);
        }
        
    
        
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(4000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        if(-1 == connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
            printf("접속 실패\n");
            exit(1);
        }
        
        strcpy(ip_buf, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
        //strcpy(ip_buf, "192.168.10.2"); //Hard Coding of Client IP

        
        //쓰기
        printf("[%s -> send] ", ip_buf);
        scanf("%s", message);
        
        strcpy(totalMessage, "[");
        strcat(totalMessage, ip_buf);
        strcat(totalMessage, " -> rcv] ");
        strcat(totalMessage, message);
        
        if(!strcmp(message, "bye"))
            break;
        
        write(client_socket, totalMessage, strlen(totalMessage) + 1);
        

        //읽기
        read(client_socket, buff, BUFF_SIZE);
        printf("%s\n", buff);

    }
    
    close(client_socket);

    
    return 0;
}
