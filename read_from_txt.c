#include <printf.h>
#include <sys/fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "todo_structure.h"

//todo.txt로부터 todo를 읽어온다
int read_from_txt(todo_structure todoStructure[100], char todo_txt_directory[100]) {
    int todoCount = 0; //프로그램 초기에 이미 저장된 todo를 불러오는 것이므로 count는 0이다.

    FILE *fp;

    char buf[1024];//문자열을 읽을 버퍼
    fp = fopen(todo_txt_directory, "rt"); //고수준 파일 입출력
    if (fp==NULL) {
        return 0;
    }//파일을 엶

    char tok1[32], tok2[32], tok3[32];

    while (NULL != fgets(buf, sizeof(buf), fp)) { //todo.txt에서 각 라인의 데이터를 todoStructure 구조체 배열에 저장
        sscanf(buf, "%s %s %s", tok1, tok2, tok3);
        strcpy(todoStructure[todoCount].todo, tok1);
        strcpy(todoStructure[todoCount].tag, tok2);
        todoStructure[todoCount].done = atoi(tok3);

        todoCount++;
    }
    fflush(fp);
    fclose(fp);

    return todoCount;
}