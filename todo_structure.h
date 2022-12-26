#ifndef __TODO_STRUCTURE_H__
#define __TODO_STRUCTURE_H__

typedef struct todo_structure { //각todo가 저장되는 구조체
    char todo[50];//할일 저장하는 배열
    char tag[50];//파일 태그
    char done;//완료 여부
}todo_structure;

#endif