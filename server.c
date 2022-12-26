#include <stdio.h>//표준 출력
#include <stdlib.h>//표존 라이브러리
#include <string.h>//문자열 라이브러리
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAXLINE  511
#define MAX_SOCK 1024 // 솔라리스의 경우 64

static char *EXIT_STRING = "exit";	// 클라이언트의 종료 요청 문자열
char *START_STRING = "Connected to chat_server \n";//서버에 연결을 알리는 문자열
// 클라이언트 환영 메시지
int maxfdp1;				// 최대 소켓번호 +1
int num_user = 0;			// 채팅 참가자 수
int num_chat = 0;			// 지금까지 오간 대화의 수
int clisock_list[MAX_SOCK];		// 채팅에 참가자 소켓번호 목록
char ip_list[MAX_SOCK][20];		//접속한 ip목록
int listen_sock;			// 서버의 리슨 소켓

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr);
int getmax();				// 최대 소켓 번호 찾기
void removeClient(int s);	// 채팅 탈퇴 처리 함수
int server_tcp_listen(int host, int port, int backlog); // 소켓 생성 및 listen

void server_errquit(char *mesg) { perror(mesg); exit(1); }

time_t ct;
struct tm tm;

void *thread_function(void *arg) { //명령어를 처리할 스레드
    int i;
    printf("명령어 목록 : help, num_user, num_chat, ip_list, exit\n");
    while (1) {
        char bufmsg[MAXLINE + 1];
        fprintf(stderr, "\033[1;32m"); //글자색을 녹색으로 변경
        printf("server>"); //커서 출력
        fgets(bufmsg, MAXLINE, stdin); //명령어 입력
        if (!strcmp(bufmsg, "\n")) continue;   //엔터 무시
        else if (!strcmp(bufmsg, "help\n"))    //명령어 처리
            printf("help, num_user, num_chat, ip_list\n");
        else if (!strcmp(bufmsg, "num_user\n"))//명령어 처리
            printf("현재 참가자 수 = %d\n", num_user);
        else if (!strcmp(bufmsg, "num_chat\n"))//명령어 처리
            printf("지금까지 오간 대화의 수 = %d\n", num_chat);
        else if (!strcmp(bufmsg, "ip_list\n")) //명령어 처리
            for (i = 0; i < num_user; i++)
                printf("%s\n", ip_list[i]);
        else if (!strcmp(bufmsg, "exit\n")){
            exit(1); //명령어 처리
        }else //예외 처리
            printf("해당 명령어가 없습니다.help를 참조하세요.\n");
    }
}

int server_main(int argc, char *argv[]) {

    struct sockaddr_in cliaddr;//클라이언트의 ip를 저장하기 위해서 사용할 구조체 ipv4

    char buf[MAXLINE + 1]; //클라이언트에서 받은 메시지
    int i, j, nbyte, accp_sock;

    socklen_t addrlen = sizeof(struct sockaddr_in);

    fd_set read_fds;	//읽기를 감지할 fd_set 구조체
    pthread_t a_thread;//쓰레드를 생성하기 위한 구조체

    if (argc != 2) {//터미널에서 port 번호를 입력하지 않았다면 출력한다.
        printf("사용법 :%s port\n", argv[0]);
        exit(0);
    }

    // tcp_listen(host, port, backlog) 함수 호출
    listen_sock = server_tcp_listen(INADDR_ANY, atoi(argv[1]), 5);//샐행할 때 포트 변호를 지정해서 사용하게 만들었음. 9999
    //INADDR_ANY - 오든 렌타카드로 부터 오는 데이터를 여기서 처리함.
    //최대 사용자 5명을 지정함.


    //스레드 생성
    pthread_create(&a_thread, NULL, thread_function, (void *)NULL);
    //


    while (1) {

        FD_ZERO(&read_fds);//Fd set 초기화 그러니까 소켓을 저장할 셋을 0으로 초기화 한거임

        FD_SET(listen_sock, &read_fds);//fd set에 listen_socket을 read_fds를 추가해서 감시를 선언함.

        for (i = 0; i < num_user; i++)
            FD_SET(clisock_list[i], &read_fds);//이후 생성된 클라이언트 소켓을 모두 fd_set에 주입함

        maxfdp1 = getmax() + 1;	// maxfdp1 재 계산

        if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
            server_errquit("select fail");

        if (FD_ISSET(listen_sock, &read_fds)) {

            accp_sock = accept(listen_sock,
                               (struct sockaddr*)&cliaddr, &addrlen);

            if (accp_sock == -1) server_errquit("accept fail");

            addClient(accp_sock, &cliaddr);

            send(accp_sock, START_STRING, strlen(START_STRING), 0);

            ct = time(NULL);			//현재 시간을 받아옴

            tm = *localtime(&ct);

            write(1, "\033[0G", 4);		//커서의 X좌표를 0으로 이동
            printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(stderr, "\033[33m");//글자색을 노란색으로 변경
            printf("사용자 1명 추가. 현재 참가자 수 = %d\n", num_user);
            fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
            fprintf(stderr, "server>"); //커서 출력
        }

        // 클라이언트가 보낸 메시지를 모든 클라이언트에게 방송
        for (i = 0; i < num_user; i++) {
            if (FD_ISSET(clisock_list[i], &read_fds)) {

                num_chat++;				//총 대화 수 증가
                nbyte = recv(clisock_list[i], buf, MAXLINE, 0);

                if (nbyte <= 0) {
                    removeClient(i);	// 클라이언트의 종료
                    continue;
                }

                buf[nbyte] = 0;
                // 종료문자 처리

                if (strstr(buf, EXIT_STRING) != NULL) {
                    removeClient(i);	// 클라이언트의 종료
                    continue;
                }

                // 모든 채팅 참가자에게 메시지 방송
                for (j = 0; j < num_user; j++)
                    send(clisock_list[j], buf, nbyte, 0);
                printf("\033[0G");		//커서의 X좌표를 0으로 이동
                fprintf(stderr, "\033[97m");//글자색을 흰색으로 변경
                printf("%s", buf);			//메시지 출력
                fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
                fprintf(stderr, "server>"); //커서 출력
            }
        }

    }  // end of while
    exit(getpid());
    return 0;
}

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr) {

    char buf[20];

    inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));

    write(1, "\033[0G", 4);		//커서의 X좌표를 0으로 이동
    fprintf(stderr, "\033[33m");	//글자색을 노란색으로 변경
    printf("new client: %s\n", buf);//ip출력

    // 채팅 클라이언트 목록에 추가
    clisock_list[num_user] = s;
    strcpy(ip_list[num_user], buf);
    num_user++; //유저 수 증가

}

// 채팅 탈퇴 처리
void removeClient(int s) {

    close(clisock_list[s]);//클라이언트에서 생성된 소켓을 닫는다.

    if (s != num_user - 1) { //저장된 리스트 재배열
        clisock_list[s] = clisock_list[num_user - 1];//제일 끝에 저장된 클라이언트의 sock을 탈퇴한 소켓이 들어있는 자리에 옮김
        strcpy(ip_list[s], ip_list[num_user - 1]);//이후 문자열로 저장된 클라이언트의 ip를 덮어쓴다.
    }

    num_user--; //유저 수 감소

    ct = time(NULL);			//현재 시간을 받아옴
    tm = *localtime(&ct);

    write(1, "\033[0G", 4);		//커서의 X좌표를 0으로 이동
    fprintf(stderr, "\033[33m");//글자색을 노란색으로 변경
    printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_user);
    fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
    fprintf(stderr, "server>"); //커서 출력

}

// 최대 소켓번호 찾기
int getmax() {
    // Minimum 소켓번호는 가정 먼저 생성된 listen_sock
    int max = listen_sock;
    int i;
    for (i = 0; i < num_user; i++)
        if (clisock_list[i] > max)
            max = clisock_list[i];
    return max;
}

// listen 소켓 생성 및 listen
int  server_tcp_listen(int host, int port, int backlog) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);//TCP 파일 관리자를 생성함.

    if (sd == -1) {//오류 메세지 출력
        perror("socket fail");
        exit(1);
    }

    // servaddr 구조체의 내용 세팅
    bzero((char *)&servaddr, sizeof(servaddr));//크기 만큼 0으로 채움

    servaddr.sin_family = AF_INET;//아피 주소 저장 방식 지정함
    servaddr.sin_addr.s_addr = htonl(host);//호스트 정보를 네트워크 바이너리 순서로 변환함
    servaddr.sin_port = htons(port);//포트번호 변환함

    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");  exit(1);
    }

    // 클라이언트로부터 연결요청을 기다림
    listen(sd, backlog);//첫번째 파라미터로는 socket함수가 생성한 소켓 기술자 두번째는 최대 허용 클라이언트 가 지정된다.
    //지정한 소켓으로 만흗ㅁ
    return sd;
}
