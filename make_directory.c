#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void make_directory(char semester_folder[100],char desktop_directory[100]) {
    char semester[50];
    printf("-----정보입력-----\n");
    printf("현재 학기 입력: ");
    scanf("%s", semester);
    getchar();

    strcpy(semester_folder,desktop_directory);
    strcat(semester_folder,"/");
    strcat(semester_folder,semester); //semester_folder = desktop_directory + / +semester
    if (mkdir(semester_folder, 0777) == -1) { //폴더 만들기
        printf("이미 존재하는 폴더입니다.\n");
        return;
    }
    printf("최상위폴더 생성: %s/%s\n", desktop_directory, semester);

    //현재 수강중인 과목의 이름을 입력받아 semester명으로 생성한 폴더 아래에 폴더 만들기
    printf("현재 수강중인 과목의 수 입력: ");
    int count = 0;
    char subject_name[50];
    scanf("%d", &count);
    getchar();
    for (int i = 0; i < count; i++) {
        printf("현재 수강중인 과목명 입력: ");
        scanf("%s", subject_name);
        getchar();
        printf("폴더 생성: %s/%s\n", semester_folder, subject_name);
        char semester_temp[100];//semester값이 변하지 않도록 복사해서 사용
        strcpy(semester_temp, semester_folder);
        strcat(semester_temp, "/");
        strcat(semester_temp, subject_name);
        if (mkdir(semester_temp, 0755) == -1) {
            perror("subject_folder");
            exit(1);
        }
    }
    return;
}
