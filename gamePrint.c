#include "gamePrint.h"

int main(void) {
    FILE* fp = fopen("Tutorial.txt", "r");

    if (fp == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);

    return 0;
}
