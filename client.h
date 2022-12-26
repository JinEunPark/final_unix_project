//
// Created by 박진은 on 2022/12/14.
//

#ifndef UNIX_NEW_CLIENT_H
#define UNIX_NEW_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAXLINE     1000
#define NAME_LEN    20

char *EXIT_STRING = "exit";
// 소켓 생성 및 서버 연결, 생성된 소켓리턴
int client_tcp_connect(int af, char *servip, unsigned short port);

void errquit_client(char *mesg);

void todotxt_to_string(char todo_txt_directory[],char todo_string[]);

int client_main(int argc, char *argv[], char name[], char todotxtdir[]);




#endif //UNIX_NEW_CLIENT_H
