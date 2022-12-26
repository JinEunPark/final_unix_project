//
// Created by 구재성 on 2022/12/09.
//

#include <stdio.h>
#include <string.h>
#include "upload_to_txt.h"
#include "todo_structure.h"

int delete_todo(todo_structure todoStructure[], int todo_count,char todo_txt_directory[100]) {
    printf("---Todo 삭제---\n");
    if (todo_count == 0) {
        printf("삭제할 항목이 없습니다.\n");
        return -1;
    }

    int select;// todo 를 선택하는 변수

    select_error://goto label

    printf("삭제할 todo를 선택하시오\n");//

    for (int i = 0; i < todo_count; i++) {//todo리스트 프린트
        printf("%d. %s %s %d\n", i + 1, todoStructure[i].todo, todoStructure[i].tag, todoStructure[i].done);
    }

    scanf("%d", &select);
    getchar();

    if (select < 1 | select > todo_count)
        goto select_error;

    for (select; select < todo_count; select++) { //todo 삭제

        strcpy(todoStructure[select-1].todo, todoStructure[select].todo);
        strcpy(todoStructure[select-1].tag, todoStructure[select].tag);
        todoStructure[select-1].done=todoStructure[select].done;
    }

    todo_count--;//todo 개수를 줄임

    upload_to_txt(todoStructure, todo_count, todo_txt_directory); //todo.txt에 업데이트.

    printf("삭제 완료\n");

    return todo_count;
}