#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

int main() {
    displayWelcomeScreen();

    int mode;
    printf("원하시는 모드를 선택하세요 (1: 솔로, 2: 모드2, 3: 방 만들기, 0: 종료): ");
    scanf("%d", &mode);

    switch (mode) {
    case 1 : 
        playMode1(); // 튜토리얼
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
        printf("게임을 종료합니다.");
        break;
    default:
        printf("!");
    }
    

    return 0;
}
