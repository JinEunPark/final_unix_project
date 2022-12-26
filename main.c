#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "make_todo.h"
#include "make_directory.h"
#include "delete_todo.h"
#include "upload_subject.h"
#include "view_file.h"
#include "todo_structure.h"
#include "read_from_txt.h"
#include "upload_to_txt.h"
#include "client.h"
#include "server.h"


int main(int argc, char* argv[]) {
    printf("*****과제관리시스템*****\n");
    printf("교수 1번 입력 or 학생 2번 입력하세요: ");

    int proOrStudent;//교수인지 학생인지 테스트하는 변수
    scanf("%d",&proOrStudent);
    getchar();


    if(proOrStudent == 1){

        printf("학생들의 해야할일을 확인해보세요\n");
        printf("학생들이 프로그램을 종료하면 현황을 받아 보실 수 있습니다!!\n");
        argc = 2;
        argv[1] = "9999";
        server_main(argc,argv);

    }else{


        char name[20];
        char desktop_directory[100];//바탕화면의 절대경로
        char semester_folder[100];//학기명(ex)2022-2) 폴더의 절대경로
        char todo_txt_directory[100];//todo.txt가 있는 경로. todo.txt는 바탕화면에 생성됨.
        printf("이름을 입력해주세요: ");
        scanf("%s",name);
        getchar();
        printf("바탕화면의 절대경로를 입력해주세요: ");
        scanf("%s", desktop_directory);
        getchar();

        make_directory(semester_folder,desktop_directory);


        strcpy(todo_txt_directory, desktop_directory);
        strcat(todo_txt_directory, "/todo.txt");//todo_txt_directory 는 바탕화면+"todo.txt"이다

        todo_structure todoStructure[100];//todo가 저장되는 구조체 배열
        int todo_count = read_from_txt(todoStructure, todo_txt_directory); //과거에 저장한 todo를 불러온다.

        printf("todocount = %d\n", todo_count);
        for (int i = 0; i < todo_count; i++) { //과거에 저장한 todo를 프린트한다.
            printf("%d. done: %d / %s / %s\n", i + 1, todoStructure[i].done, todoStructure[i].todo,
                   todoStructure[i].tag);
        }

        while (1) {
            printf("---------------\n");
            printf("1. Todo 추가   2.Todo 삭제   3.과제 업로드   4.todo확인    5. 파일 확인    6.종료\n"); //메뉴
            int menu_select; //메뉴중 선택한 번호를 저장한다.
            //getchar();
            scanf("%d", &menu_select);
            getchar();
            printf("선택한 메뉴: %d\n",menu_select);

            switch (menu_select) {
                case 1: //todo 생성
                    todo_count = make_todo(todoStructure, todo_count,todo_txt_directory);
                    break;
                case 2: //todo 삭제
                    todo_count = delete_todo(todoStructure, todo_count, todo_txt_directory);
                    for (int i = 0; i < todo_count; i++) {
                        printf("%d. done: %d / %s / %s\n", i + 1, todoStructure[i].done, todoStructure[i].todo,
                               todoStructure[i].tag);
                    }
                    break;
                case 3: //과제 업로드
                    todo_count = upload_subject(desktop_directory, semester_folder, todoStructure, todo_count,todo_txt_directory);
                    break;
                case 4: //현재 todo리스트 확인
                    for (int i = 0; i < todo_count; i++) {
                        printf("%d. done: %d / %s / %s\n", i + 1, todoStructure[i].done, todoStructure[i].todo,
                               todoStructure[i].tag);
                    }
                    break;
                case 5:
                    view_file(semester_folder);
                    break;
                case 6:
                    argc = 4;
                    argv[1] = "127.0.0.1";
                    argv[2] = "9999";
                    argv[3] = name;
                    fflush(stdin);
                    client_main(argc, argv,name, todo_txt_directory);
                    exit(1);
                    break;
            }
        }

    }





}
