//
// Created by 구재성 on 2022/12/09.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//signal handler 지정
void sig_handler(int signo){
    psignal(signo,"\n파일를 읽는 프로세스가 종료 되었습니다.");
}
//semester_folder에 저장된 파일의 목록을 본다.
void view_file(char semester_folder[100]){ //semester folder는 학기명으로 생성한 폴더의 절대경로

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGALRM);
    act.sa_flags = 0;
    act.sa_handler = sig_handler;

    if((sigaction(SIGALRM,&act,(struct sigaction *)NULL))<0){
        perror("sigaction");
        exit(1);
    }

    if(chdir(semester_folder)==-1){
        perror("chdir");
        exit(1);
    }
    printf("폴더를 선택하세요(문자열 입력)\n");
    int status;
    pid_t pid;
    switch(pid=fork()){
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            if(execlp("ls","ls",(char *)NULL)==-1){ //execlp()함수를 통해 셸에서 파일 목록 출력
                perror("execlp");
                exit(1);
            }
            break;
        default:
            wait(&status);
            break;
    }

    char select[50]; //선택한 폴더의 이름을 입력하는 변수
    scanf("%s",select);
    getchar();

    //to_change_folder에 변경할 디렉토리의 절대경로를 저장한다.
    char to_change_folder[100];
    strcpy(to_change_folder, semester_folder);
    strcat(to_change_folder, "/");
    strcat(to_change_folder, select);

    if(chdir(to_change_folder)==-1){
        perror("chdir");
        exit(1);
    }

    switch(pid=fork()){ //자식프로세스 생성
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            if(execlp("ls","ls",(char *)NULL)==-1){ //execlp()함수를 통해 셸에서 파일 목록 출력
                perror("execlp");
                exit(1);
            }
            exit(1);

            break;
        default:
            wait(&status);
            break;
    }
    alarm(1);//파일 읽는 프로세스가 완료되었다고 알림을 보냄.


    //main함수로 돌아가기 전 단계
    int ret;
    _ret:
    printf("돌아가기: 1 입력");
    scanf("%d",&ret);
    getchar();
    if(ret==1) return;
    else sleep(5);
    goto _ret;

}