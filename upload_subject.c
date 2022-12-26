//
// Created by 구재성 on 2022/12/08.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include "upload_to_txt.h"
#include "todo_structure.h"

//바탕화면에 파일을 올리면 filename으로 파일을 찾아 tag에 맞는 폴더에 이동시킨다
int upload_subject(char desktop_directory[100], char semester_folder[100], todo_structure todoStructure[], int todo_count,
               char todo_txt_directory[100]) {
    printf("---과제 업로드---\n");

    if (todo_count == 0) {
        printf("todo리스트가 비어있습니다.\n");
        return -1;
    }
    for (int i = 0; i < todo_count; i++) { //todo 출력
        printf("%d. done: %d / %s / %s\n", i + 1, todoStructure[i].done, todoStructure[i].todo, todoStructure[i].tag);
    }
    printf("업로드할 항목 번호 입력: ");
    int select; //선택한 번호를 저장하는 변수
    scanf("%d", &select);
    getchar();
    if (todoStructure[select - 1].done == 1) { //done이 1이면 이미 업로드 된 todo이므로 return한다
        printf("이미 업로드된 todo입니다.\n");
        return todo_count;
    }


    if_file_open_fail:
    printf("파일의 이름을 입력해주세요: ");
    char filename[100];
    scanf("%s", filename);
    getchar();
    //현재 경로를 바꾸고 filename파일이 존재하는지 검색
    if (chdir(desktop_directory) == -1) {
        perror("chdir");
        exit(1);
    }
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) { //업로드할 파일이 존재하는지 검사
        perror("file open");
        goto if_file_open_fail;
    }
    close(fd);

    todoStructure[select - 1].done = 1; //파일을 업로드 했으므로 done==1
    printf("파일 이동중...");
    //exec함수를 사용해 바탕화면에 있는 filename파일을 semester/tag폴더로 이동.
    int status;
    pid_t pid;
    switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            printf("child process");
            char destination[100];
            //destination문자열에 이동 대상 폴더의 절대경로를 저장
            strcpy(destination, semester_folder);
            strcat(destination, "/");
            strcat(destination, todoStructure[select - 1].tag);
            strcat(destination, "/");
            printf("%s", destination);

            if (execlp("mv", "mv", filename, destination, (char *) NULL) == -1) {//execlp()함수를 이용해 업로드할 파일을 이동한다
                perror("execlp__");
                exit(1);
            }
            exit(1);

            break;
        default:
            wait(&status);
            break;
    }

    printf("업로드 완료: %s/%s/%s\n", semester_folder, todoStructure[select - 1].tag, filename);
    todoStructure[select - 1].done = 1; //업로드 완료한 todo의 done ==1
    upload_to_txt(todoStructure, todo_count, todo_txt_directory);//todo.txt에 업데이트

    return todo_count;
}
