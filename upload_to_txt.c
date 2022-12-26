#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "todo_structure.h"

//저수준 파일 입출력을 통해 todo, tag, done을 filename.txt에 저장.
int upload_to_txt(todo_structure todoStructure[], int todo_count, char todo_txt_directory[100]) {

    int fd_todotxt;
    fd_todotxt = open(todo_txt_directory, O_CREAT | O_WRONLY | O_TRUNC, 0644); //todo.txt파일을 연다
    if (fd_todotxt == -1) {
        perror("creat");
        exit(1);
    }


    //todo, tag, done을 문자열로 만들어 todo.txt에 저장한다. 각 요소의 구분은 스페이스로 한다.
    int i = 0;
    while (i < todo_count) {
        int string_size = 0;
        int k = 0;
        char to_insert_string[200];

        //to_insert_string = "todo tag done" 이다
        strcpy(to_insert_string, todoStructure[i].todo);
        strcat(to_insert_string, " ");
        strcat(to_insert_string, todoStructure[i].tag);
        strcat(to_insert_string, " ");

        char done_temp[5]; //done을 문자열로 변환
        sprintf(done_temp, "%d", todoStructure[i].done);
        strcat(to_insert_string, done_temp);
        strcat(to_insert_string,"\n");

        int fd[2];
        pid_t pid;
        char buf[201];
        int len, status;

        if(pipe(fd)==-1){ //파이프 생성 오류시 오류메세지 출력
            perror("pipe");
            printf("파이프 생성 오류! 정상적으로 todo.txt에 저장되지 않을 수 있습니다.");
            continue;
        }
        switch (pid=fork()) { //자식프로세스 생성
            case -1: //fork 오류
                perror("fork");
                exit(1);
                break;
            case 0: //Child
                close(fd[1]); //쓰기 파이프를 닫는다
                len=read(fd[0],buf,200); //buf에 파이프에서 받아온 문자열 저장
                write(fd_todotxt, buf, len); //buf에 저장된 문자열을 todo.txt에 출력
                close(fd[0]); //읽기 파이프를 닫는다.
                exit(1);
                break;
            default:
                close(fd[0]); //읽기 파이프를 닫는다.
                write(fd[1],to_insert_string,strlen(to_insert_string)); //to_insert_string을 파이프로 보낸다.
                close(fd[1]); //쓰기 파이프를 닫는다.
                waitpid(pid,&status,0);
                break;
        }
        i++;
    }
    close(fd_todotxt);
    return 0;
}
