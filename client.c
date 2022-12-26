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
#include "todo_structure.h"

#define MAXLINE     1000
#define NAME_LEN    20

static char *EXIT_STRING = "exit";
// 소켓 생성 및 서버 연결, 생성된 소켓리턴
int client_tcp_connect(int af, char *servip, unsigned short port);
void errquit_client(char *mesg) { perror(mesg); exit(1); }

void todotxt_to_string(char todo_txt_directory[],char todo_string[]) {
    FILE *fp;

    char buf[1024];//문자열을 읽을 버퍼
    fp = fopen(todo_txt_directory, "rt");
    if (fp==NULL) {
        return;
    }//파일을 엶


    while (NULL != fgets(buf, sizeof(buf), fp)) { //todo.txt에서 각 라인의 데이터를 todoStructure 구조체 배열에 저장
        strcat(todo_string,buf);
    }
    printf("%s",todo_string);
    fflush(fp);
    fclose(fp);

}

int client_main(int argc, char *argv[], char name[], char todotxtdir[]) {
    char todo_string[1024]={0};
    todotxt_to_string(todotxtdir,todo_string);

    char bufname[NAME_LEN];	// 이름
    strcpy(bufname,name);
    char bufmsg[MAXLINE];	// 메시지부분
    char bufall[MAXLINE + NAME_LEN];
    int maxfdp1;	// 최대 소켓 디스크립터

    int s;		// 소켓
    int namelen;	// 이름의 길이

    fd_set read_fds;//socket을 저장하기 위해서 사용하는 것
    time_t ct;

    struct tm tm;

    if (argc != 4) {//port name 을 입력하지 않은 경우
        printf("사용법 : %s sever_ip  port name \n", argv[0]);
        exit(0);
    }

    //서버에 연결을 요청하는 함수 main에 전달된 포트번호와 ip를 이용해서 연결을 요청함.
    s = client_tcp_connect(AF_INET, argv[1], atoi(argv[2]));

    if (s == -1)
        errquit_client("tcp_connect fail");

    puts("서버에 접속되었습니다.");
    maxfdp1 = s + 1;


    FD_ZERO(&read_fds);
        char tmp[100];
        strcat(tmp,"학생이름: ");
        strcat(tmp,name);
        strcat(tmp,"\n");


        sprintf(bufall,tmp);//메시지에 현재시간 추가
        if (send(s, bufall, strlen(bufall), 0) < 0)
            puts("Error : Write error on socket.");

        sprintf(bufall,"\n파일이름 태그 완료여부\n");//메시지에 현재시간 추가
        if (send(s, bufall, strlen(bufall), 0) < 0)
            puts("Error : Write error on socket.");

        sprintf(bufall,todo_string);//메시지에 현재시간 추가
        if (send(s, bufall, strlen(bufall), 0) < 0)
            puts("Error : Write error on socket.");



    while (1) {
        FD_SET(0, &read_fds);
        FD_SET(s, &read_fds);

        if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
            errquit_client("select fail");

        if (FD_ISSET(s, &read_fds)) {

            int nbyte;
            if ((nbyte = recv(s, bufmsg, MAXLINE, 0)) > 0) {

                bufmsg[nbyte] = 0;

                write(1, "\033[0G", 4);		//커서의 X좌표를 0으로 이동
                printf("%s", bufmsg);		//메시지 출력
                fprintf(stderr, "\033[1;32m");	//글자색을 녹색으로 변경
                fprintf(stderr, "%s>", argv[3]);//내 닉네임 출력

            }

        }

        if (FD_ISSET(0, &read_fds)) {
            if (fgets(bufmsg, MAXLINE, stdin)) {
                fprintf(stderr, "\033[1;33m"); //글자색을 노란색으로 변경
                fprintf(stderr, "\033[1A"); //Y좌표를 현재 위치로부터 -1만큼 이동
                ct = time(NULL);	//현재 시간을 받아옴
                tm = *localtime(&ct);
                sprintf(bufall, "[%02d:%02d:%02d]%s>%s", tm.tm_hour, tm.tm_min, tm.tm_sec, argv[3], bufmsg);//메시지에 현재시간 추가

                if (send(s, bufall, strlen(bufall), 0) < 0)
                    puts("Error : Write error on socket.");

                if (strstr(bufmsg, EXIT_STRING) != NULL) {
                    puts("Good bye.");
                    close(s);
                    exit(0);
                }

            }
        }

    } // end of while
}

int client_tcp_connect(int af, char *servip, unsigned short port) {

    struct sockaddr_in servaddr;
    int  s;

    // 소켓 생성
    if ((s = socket(af, SOCK_STREAM, 0)) < 0)
        return -1;

    // 채팅 서버의 소켓주소 구조체 servaddr 초기화
    bzero((char *)&servaddr, sizeof(servaddr));

    servaddr.sin_family = af;
    inet_pton(AF_INET, servip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    // 연결요청
    if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr))
        < 0)
        return -1;

    return s;
}



