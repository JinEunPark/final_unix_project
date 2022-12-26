//
// Created by 구재성 on 2022/12/08.
//
#include <stdio.h>
#include "upload_to_txt.h"
#include "todo_structure.h"

//todo를 만들고 todo.txt파일에 저장한다.

int make_todo(todo_structure todoStructure[], int todo_count,char todo_txt_directory[100]) {//현재 todo리스트에 저장된 개수를 받아온다.

    printf("---Todo 추가---\n");
    printf("내용 입력: ");
    scanf("%s", todoStructure[todo_count].todo);//todo명 입력
    getchar();

    printf("태그 입력: ");
    scanf("%s", todoStructure[todo_count].tag);//todo tag입력
    getchar();

    todoStructure[todo_count].done = 0;//새로 생성된 todo이므로 done==0

    todo_count++;//todo 개수를 1 증가
    upload_to_txt(todoStructure, todo_count, todo_txt_directory); //todo.txt에 업데이트

    return todo_count;
}
