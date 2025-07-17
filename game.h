// game.h
#ifndef GAME_H
#define _CRT_SECURE_NO_WARNINGS
void playMode1();

/*
* 아래 3개의 함수는 playMode2 안에 create, conn 기능 종속으로 들어감
* 나중에 파일 분리 할때 game에 통합할 예정 (현재 시스템에서는 main에 멀티 플레이가 소켓으로 구현되어 있음)
*/
//void playMode2();
//void createRoom();
//void connRomm();
#endif // GAME_H
