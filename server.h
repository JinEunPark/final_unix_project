//
// Created by 박진은 on 2022/12/14.
//

#ifndef UNIX_NEW_SERVER_H
#define UNIX_NEW_SERVER_H



// 새로운 채팅 참가자 처리

void errquit_client(char *mesg);

void *thread_function(void *arg);//명령어를 처리할 스레드



int server_main(int argc, char *argv[]);


// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr);

// 채팅 탈퇴 처리
void removeClient(int s);

// 최대 소켓번호 찾기
int getmax();

// listen 소켓 생성 및 listen
int  tcp_listen(int host, int port, int backlog);


#endif //UNIX_NEW_SERVER_H
