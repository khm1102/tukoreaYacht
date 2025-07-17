#include "gamePrint.h"

void printTutorial() {

    FILE* fp = fopen("Tutorial.txt", "r");

    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
}