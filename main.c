#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#pragma comment(lib, "ws2_32.lib")

#define PORT 5555


SOCKET setupServer() {
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) { perror("socket"); exit(1); }
    struct sockaddr_in serv = { AF_INET, htons(PORT), {INADDR_ANY}, 0 };
    if (bind(listenSock, (struct sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) exit(1);
    if (listen(listenSock, 1) == SOCKET_ERROR) exit(1);
    printf("방 만들기: 포트 %d 대기 중...\n", PORT);
    SOCKET clientSock = accept(listenSock, NULL, NULL);
    closesocket(listenSock);
    printf("클라이언트 접속!\n");
    return clientSock;
}

SOCKET setupClient(const char* ip) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) { perror("socket"); exit(1); }
    struct sockaddr_in serv = { AF_INET, htons(PORT), {0}, 0 };
    inet_pton(AF_INET, ip, &serv.sin_addr);
    printf("서버(%s:%d)에 연결 중...\n", ip, PORT);
    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) exit(1);
    printf("서버 연결 성공!\n");
    return sock;
}

void doLocalTurn(SOCKET sock, int score[], const char* who) {
    int dice[NUM_DICE], keep[NUM_DICE];
    for (int i = 0; i < NUM_DICE; i++) keep[i] = 0;
    printf("\n[%s의 턴]\n", who);
    for (int r = 0; r < 3; r++) {
        rollDice(dice, keep);
        send(sock, (char*)dice, sizeof(dice), 0);
        send(sock, (char*)keep, sizeof(keep), 0);
        displayDice(dice);
        if (r < 2) dice_S(dice, keep);
    }
    print_PS(score);
    int cat;
    do {
        printf("카테고리 선택 (1~12): "); scanf("%d", &cat);
    } while (cat < 1 || cat > NUM_CATEGORIES || score[cat - 1] != -1);
    cat--;
    send(sock, (char*)&cat, sizeof(cat), 0);
    score[cat] = calc_score(cat, dice);
    printf("%s 점수: %d\n", who, score[cat]);
}

void doRemoteTurn(SOCKET sock, int score[], const char* who) {
    int dice[NUM_DICE], keep[NUM_DICE];
    printf("\n[%s의 턴 (관전)]\n", who);
    for (int r = 0; r < 3; r++) {
        recv(sock, (char*)dice, sizeof(dice), 0);
        recv(sock, (char*)keep, sizeof(keep), 0);
        displayDice(dice);
        Sleep(500);
    }
    int cat;
    recv(sock, (char*)&cat, sizeof(cat), 0);
    score[cat] = calc_score(cat, dice);
    printf("%s 카테고리: %d  점수: %d\n", who, cat + 1, score[cat]);
}

void playNetworkGame(SOCKET sock, int isServer) {
    int scoreSelf[NUM_CATEGORIES], scoreOpp[NUM_CATEGORIES];
    for (int i = 0; i < NUM_CATEGORIES; i++)
        scoreSelf[i] = scoreOpp[i] = -1;
    srand((unsigned)time(NULL) + isServer);

    const char* me = isServer ? "서버(나)" : "클라이언트(나)";
    const char* opponent = isServer ? "클라이언트" : "서버";

    for (int round = 0; round < NUM_CATEGORIES; round++) {
        if (isServer)      doLocalTurn(sock, scoreSelf, me);
        else               doRemoteTurn(sock, scoreOpp, opponent);

        if (!isServer)     doLocalTurn(sock, scoreSelf, me);
        else               doRemoteTurn(sock, scoreOpp, opponent);
    }

    // 결과 출력
    printf("\n=== 최종 점수 ===\n");
    printf("%s : ", me);     print_PS(scoreSelf);
    printf("%s : ", opponent); print_CS(scoreOpp);
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { printf("WSAStartup 실패\n"); return 1; }

    while (1) {
        displayWelcomeScreen();
        printf("모드를 선택하세요:\n");
        printf(" 1: 솔로  2: 네트워크  0: 종료\n");
        int mode; scanf("%d", &mode);

        if (mode == 1) {
            playMode1();
        }
        else if (mode == 2) {
            printf(" 1: 방 만들기  2: 방 참가\n");
            int netMode; scanf("%d", &netMode);

            SOCKET sock = INVALID_SOCKET;
            if (netMode == 1) {
                sock = setupServer();
                playNetworkGame(sock,1);
            }
            else if (netMode == 2) {
                char ip[64];
                printf("서버 IP 입력: "); scanf("%63s", ip);
                sock = setupClient(ip);
                playNetworkGame(sock,0);
            }

            if (sock != INVALID_SOCKET) closesocket(sock);

            // 게임 끝난 뒤 선택지
            printf("\n1: 방 나가기(메뉴로 돌아가기)  2: 프로그램 종료\n");
            int sel; scanf("%d", &sel);
            if (sel == 2) break;  // while 루프 탈출 → 종료
            // 메뉴로 복귀
        }
        else {
            printf("프로그램을 종료합니다.\n");
            break;
        }
    }

    WSACleanup();
    return 0;
}
