//
//  main.c
//  server.c
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
#include <netinet/in.h>
#include <net/if.h>

#define BUFF_SIZE 1024


int main(int argc, const char * argv[]) {
    int server_socket = 0;
    int client_socket = 0;
    unsigned int client_addr_size = 0;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    struct ifreq ifr;
    
    char buff_rcv[BUFF_SIZE];
    char ip_buf[BUFF_SIZE];
    char message[BUFF_SIZE];
    char totalMessage[BUFF_SIZE];
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == server_socket){
        printf("server socket 생성 실패\n");
        exit(1);
    }
    
    ifr.ifr_addr.sa_family = PF_INET;
    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    
    ioctl(server_socket, SIOCGIFADDR, &ifr);
    
    
    int port_num = 4000;
    
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
        printf("bind() 실행 에러\n");
        exit(1);
    }
    
    strcpy(ip_buf, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    
    printf("Server IP : %s \n", ip_buf);
    printf("PORT : %d\n", port_num);

    
    
    if(-1 == listen(server_socket, 5)){
        printf("listen()실행 실패\n");
        exit(1);
    }
    
    while(1){
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        
        if(-1 == client_socket){
            printf("클라이언트 연락 수락 실패\n");
            exit(1);
        }
        
    //읽기
        read(client_socket, buff_rcv, BUFF_SIZE);
        printf("%s\n", buff_rcv);

        //쓰기
        printf("[%s -> send] ", ip_buf);
        scanf("%s", message);

        strcpy(totalMessage, "[");
        strcat(totalMessage, ip_buf);
        strcat(totalMessage, " -> rcv] ");
        strcat(totalMessage, message);

        write(client_socket, totalMessage, strlen(totalMessage) + 1);
    }
    
    
    return 0;
}
