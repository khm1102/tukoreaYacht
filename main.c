#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "localUser.h"
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

#include "gamePrint.h"

#define PORT        5555
#define NAME_LEN    32

#ifndef NUM_DICE
#define NUM_DICE       5
#endif

#ifndef NUM_CATEGORIES
#define NUM_CATEGORIES 12
#endif

// 구조체 정의 (고정 크기 배열)
typedef struct {
    int values[NUM_DICE];   // 주사위 눈
    int keep[NUM_DICE];     // 보관 여부
} Dice;

typedef struct {
    char name[NAME_LEN];
    int scores[NUM_CATEGORIES];
} Player;

// 모든 바이트를 읽어들이는 헬퍼
int recvAll(SOCKET sock, void* buf, int totalBytes) {
    char* p = (char*)buf;
    int received = 0;
    while (received < totalBytes) {
        int r = recv(sock, p + received, totalBytes - received, 0);
        if (r <= 0) { perror("recv"); return 0; }
        received += r;
    }
    return 1;
}

// Player 초기화 (동적 할당 제거)
void initPlayer(Player* p, const char* name) {
    strncpy(p->name, name, NAME_LEN);
    p->name[NAME_LEN - 1] = '\0';
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        p->scores[i] = -1;
    }
}

// 로컬 턴
void doLocalTurn(SOCKET sock, Player* p) {
    Dice d = { {0}, {0} };
    printf("\n[%s의 턴]\n", p->name);

    for (int r = 0; r < 3; r++) {
        rollDice(d.values, d.keep);

        // 네트워크 바이트 순서로 변환
        int netValues[NUM_DICE], netKeep[NUM_DICE];
        for (int i = 0; i < NUM_DICE; i++) {
            netValues[i] = htonl(d.values[i]);
            netKeep[i] = htonl(d.keep[i]);
        }

        if (send(sock, (char*)netValues, sizeof(netValues), 0) == SOCKET_ERROR) {
            perror("send"); exit(1);
        }
        if (send(sock, (char*)netKeep, sizeof(netKeep), 0) == SOCKET_ERROR) {
            perror("send"); exit(1);
        }

        if (r < 2) dice_S(d.values, d.keep);
    }

    print_PS(p->scores);

    int cat;
    do {
        printf("카테고리 선택 (1~%d): ", NUM_CATEGORIES);
        scanf("%d", &cat);
    } while (cat < 1 || cat > NUM_CATEGORIES || p->scores[cat - 1] != -1);
    cat--;

    int netCat = htonl(cat);
    if (send(sock, (char*)&netCat, sizeof(netCat), 0) == SOCKET_ERROR) {
        perror("send"); exit(1);
    }

    p->scores[cat] = calc_score(cat, d.values);
    printf("%s 점수: %d\n", p->name, p->scores[cat]);
}

// 원격 턴
void doRemoteTurn(SOCKET sock, Player* p) {
    Dice d = { {0}, {0} };
    printf("\n[%s의 턴 (관전)]\n", p->name);

    for (int r = 0; r < 3; r++) {
        int netValues[NUM_DICE], netKeep[NUM_DICE];

        if (!recvAll(sock, netValues, sizeof(netValues))) exit(1);
        if (!recvAll(sock, netKeep, sizeof(netKeep)))   exit(1);

        for (int i = 0; i < NUM_DICE; i++) {
            d.values[i] = ntohl(netValues[i]);
            d.keep[i] = ntohl(netKeep[i]);
        }

        displayDice(d.values);
        Sleep(500);
    }

    int netCat;
    if (!recvAll(sock, &netCat, sizeof(netCat))) exit(1);
    int cat = ntohl(netCat);
    if (cat < 0 || cat >= NUM_CATEGORIES) {
        fprintf(stderr, "Invalid category received: %d\n", cat);
        exit(1);
    }

    p->scores[cat] = calc_score(cat, d.values);
    printf("%s 카테고리: %d  점수: %d\n",
        p->name, cat + 1, p->scores[cat]);
}

// 네트워크 게임 (반환값 사용 안 하므로 void)
void playNetworkGame(SOCKET sock, int isServer) {
    Player me, opp;
    initPlayer(&me, isServer ? "서버(나)" : "클라이언트(나)");
    initPlayer(&opp, isServer ? "클라이언트" : "서버");
    srand((unsigned)time(NULL) + isServer);

    for (int round = 0; round < NUM_CATEGORIES; round++) {
        if (isServer) {
            doLocalTurn(sock, &me);
            doRemoteTurn(sock, &opp);
        }
        else {
            doRemoteTurn(sock, &opp);
            doLocalTurn(sock, &me);
        }
    }

    int localData = 0, comData = 0;
    print_MS(me.scores, opp.scores, &localData, &comData);
    saveLocalData(localData, comData);
}

// 서버 소켓 설정
SOCKET setupServer() {
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) { perror("socket"); exit(1); }

    struct sockaddr_in serv = { AF_INET, htons(PORT), {INADDR_ANY}, 0 };
    if (bind(listenSock, (struct sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
        perror("bind"); exit(1);
    }
    if (listen(listenSock, 1) == SOCKET_ERROR) {
        perror("listen"); exit(1);
    }

    printf("방 만들기: 포트 %d 대기 중...\n", PORT);
    SOCKET clientSock = accept(listenSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) { perror("accept"); exit(1); }

    closesocket(listenSock);
    printf("클라이언트 접속!\n");
    return clientSock;
}

// 클라이언트 소켓 설정
SOCKET setupClient(const char* ip) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) { perror("socket"); exit(1); }

    struct sockaddr_in serv = { AF_INET, htons(PORT), {0}, 0 };
    if (inet_pton(AF_INET, ip, &serv.sin_addr) != 1) {
        fprintf(stderr, "Invalid IP: %s\n", ip);
        exit(1);
    }

    printf("서버(%s:%d)에 연결 중...\n", ip, PORT);
    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
        perror("connect"); exit(1);
    }

    printf("서버 연결 성공!\n");
    return sock;
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup 실패\n");
        return 1;
    }

    int mode;
    do {
        displayWelcomeScreen();
        printf("1: 솔로  2: 멀티  3: 튜토리얼  4: 조회  0: 종료\n");
        printf("모드를 선택하세요: ");
        scanf("%d", &mode);

        if (mode == 1) {
            playMode1();
        }
        else if (mode == 2) {
            printf("1: 방 만들기  2: 방 참가\n");
            int netMode; scanf("%d", &netMode);

            SOCKET sock = INVALID_SOCKET;
            if (netMode == 1) {
                sock = setupServer();
                playNetworkGame(sock, 1);
            }
            else if (netMode == 2) {
                char ip[64];
                printf("서버 IP 입력: "); scanf("%63s", ip);
                sock = setupClient(ip);
                playNetworkGame(sock, 0);
            }

            if (sock != INVALID_SOCKET) closesocket(sock);

            printf("\n1: 메뉴로 돌아가기  2: 종료\n");
            int sel; scanf("%d", &sel);
            if (sel == 2) break;
        }
        else if (mode == 3) {
            system("cls");
            while (1) {
                printTutorial();
                printf("\n읽기 종료: -1 입력 → ");
                int tmp; scanf("%d", &tmp);
                if (tmp == -1) break;
            }
        }
        else if (mode == 4) {
            printf("조회: 1: 플레이 수  2: 내역  3: 전체 데이터\n");
            int opt; scanf("%d", &opt);
            system("cls");
            if (opt == 1) {
                printf("플레이 수 조회 중...\n");
                printf("플레이 횟수: %d\n", localPlayCnt());
            }
            else if (opt == 2) {
                int order;
                printf("정렬: 1(오름차순) -1(내림차순)→ ");
                scanf("%d", &order);
                localPlaySort(order);
            }
            else if (opt == 3) {
                printAllUserData();
            }
            printf("\n4초 후 메인으로...");
            Sleep(4000);
        }
        else {
            printf("프로그램 종료\n");
            break;
        }
    } while (mode != 0);

    WSACleanup();
    return 0;
}
