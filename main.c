#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

int main() {
    displayWelcomeScreen();

    int mode;
    printf("���Ͻô� ��带 �����ϼ��� (1: �ַ�, 2: ���2, 3: �� �����, 0: ����): ");
    scanf("%d", &mode);

    switch (mode) {
    case 1 : 
        playMode1(); // Ʃ�丮��
        break;
    case 2:
        /*playMode2();*/
        break;
    case 3:
        //createRoom;
        break;
    case 4:
        //connRoom();
        break;
    case 0:
        printf("������ �����մϴ�.");
        break;
    default:
        printf("!");
    }
    

    return 0;
}
