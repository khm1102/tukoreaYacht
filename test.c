// main.c
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "graphics.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE 512

typedef struct {
    int dice[NUM_DICE];
    int keep[NUM_DICE];
    int category;
    int score;
} TurnData;

//----------------------------------------------------------------------
// 네트워크 헬퍼
static int recvAll(SOCKET sock, char* buf, int len) {
    int total = 0, r;
    while (total < len) {
        r = recv(sock, buf + total, len - total, 0);
        if (r <= 0) return 0;
        total += r;
    }
    return 1;
}

void sendTurnData(SOCKET sock, TurnData* td) {
    send(sock, (char*)td, sizeof(TurnData), 0);
}

int recvTurnData(SOCKET sock, TurnData* td) {
    return recvAll(sock, (char*)td, sizeof(TurnData));
}

//----------------------------------------------------------------------
// 한 플레이어 턴 (로컬)
void playLocalTurn(int scores[], TurnData* td) {
    int dice[NUM_DICE], keepArr[NUM_DICE] = { 0 };
    int roll, chosen = 1;

    // 최대 3번 굴리기
    for (roll = 0; roll < 3; roll++) {
        rollDice(dice, keepArr);
        printf("\nRoll %d:\n", roll + 1);
        displayDice(dice);
        if (roll < 2) {
            printf("Use arrows/SPACE to keep dice, ENTER to confirm.\n");
            dice_S(dice, keepArr);
        }
    }

    // 카테고리 선택
    while (chosen) {
        displayDice(dice);
        print_PS(scores);
        printf("Choose category (1-12): ");
        int cat;
        scanf("%d", &cat);
        cat--;
        if (cat >= 0 && cat < NUM_CATEGORIES && scores[cat] == -1) {
            scores[cat] = calc_score(cat, dice);
            printf("You scored %d points.\n", scores[cat]);
            td->category = cat;
            td->score = scores[cat];
            chosen = 0;
        }
        else {
            printf("Invalid or already used category.\n");
        }
    }

    // 턴데이터에 주사위 상태 저장
    memcpy(td->dice, dice, sizeof(dice));
    memcpy(td->keep, keepArr, sizeof(keepArr));
}

//----------------------------------------------------------------------
// 원격 플레이어 턴 처리 (화면에만 출력)
void processRemoteTurn(int remoteScores[], TurnData* td) {
    system("cls");
    printf("=== Opponent's Turn ===\n");
    display_DWC(td->dice, td->keep, -1);
    printf("Opponent chose category %d, scored %d points.\n",
        td->category + 1, td->score);
    remoteScores[td->category] = td->score;
    printf("Press any key to continue...\n");
    _getch();
}

//----------------------------------------------------------------------
// 호스트/클라이언트 공통 게임 루프
void playNetworkGame(SOCKET sock, int isHost) {
    int turn;
    int myScores[NUM_CATEGORIES], oppScores[NUM_CATEGORIES];
    for (turn = 0; turn < NUM_CATEGORIES; turn++) {
        TurnData td;
        if (isHost) {
            // 1) 내가 먼저
            playLocalTurn(myScores, &td);
            sendTurnData(sock, &td);

            // 2) 상대 턴 수신
            if (!recvTurnData(sock, &td)) { printf("Connection lost.\n"); return; }
            processRemoteTurn(oppScores, &td);
        }
        else {
            // 1) 상대 먼저
            if (!recvTurnData(sock, &td)) { printf("Connection lost.\n"); return; }
            processRemoteTurn(oppScores, &td);

            // 2) 내가
            playLocalTurn(myScores, &td);
            sendTurnData(sock, &td);
        }
    }

    // 최종 점수 계산 및 출력
    int totalMy = 0, totalOpp = 0;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (myScores[i] != -1) totalMy += myScores[i];
        if (oppScores[i] != -1) totalOpp += oppScores[i];
    }

    printf("\n=== Final Results ===\n");
    printf("You:      %d\nOpponent: %d\n", totalMy, totalOpp);
    if (totalMy > totalOpp)      printf("You Win!\n");
    else if (totalMy < totalOpp) printf("You Lose...\n");
    else                          printf("Draw.\n");
}

//----------------------------------------------------------------------
// 서버 소켓 준비 (호스트)
SOCKET setupHost(const char* port) {
    WSADATA wsa;
    SOCKET listenSock = INVALID_SOCKET, clientSock = INVALID_SOCKET;
    struct addrinfo hints = { 0 }, * res = NULL;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return INVALID_SOCKET;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res) != 0) return INVALID_SOCKET;
    listenSock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenSock == INVALID_SOCKET) return INVALID_SOCKET;
    if (bind(listenSock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
        return INVALID_SOCKET;
    if (listen(listenSock, 1) == SOCKET_ERROR) return INVALID_SOCKET;

    printf("Waiting for opponent on port %s...\n", port);
    clientSock = accept(listenSock, NULL, NULL);
    closesocket(listenSock);
    return clientSock;
}

// 클라이언트 소켓 준비 (조인)
SOCKET setupClient(const char* ip, const char* port) {
    WSADATA wsa;
    SOCKET sock = INVALID_SOCKET;
    struct addrinfo hints = { 0 }, * res = NULL;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return INVALID_SOCKET;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(ip, port, &hints, &res) != 0) return INVALID_SOCKET;
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;
    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
        return INVALID_SOCKET;
    return sock;
}

//----------------------------------------------------------------------
// main()
int main() {
    int mode;

    displayWelcomeScreen();
    srand((unsigned)time(NULL));

    printf("Select mode:\n");
    printf(" 1) Single play\n");
    printf(" 2) VS Computer\n");
    printf(" 3) Multiplayer\n");
    printf("Enter choice: ");
    scanf("%d", &mode);

    if (mode == 1) {
        // 기존 싱글 플레이 루프
        // (본래 main()의 mode==1 코드 전체를 여기에 복사)
    }
    else if (mode == 2) {
        // 기존 VS Computer 루프
        // (본래 main()의 mode==2 코드 전체를 여기에 복사)
    }
    else if (mode == 3) {
        int netChoice;
        char ip[128], port[6];
        SOCKET sock = INVALID_SOCKET;

        printf(" Multiplayer:\n");
        printf(" 1) Host game\n");
        printf(" 2) Join game\n");
        printf("Select: ");
        scanf("%d", &netChoice);

        printf("Enter port: ");
        scanf("%5s", port);

        if (netChoice == 1) {
            sock = setupHost(port);
            if (sock == INVALID_SOCKET) { printf("Host setup failed.\n"); return 1; }
            printf("Opponent connected!\n");
            playNetworkGame(sock, 1);
        }
        else {
            printf("Enter host IP: ");
            scanf("%127s", ip);
            sock = setupClient(ip, port);
            if (sock == INVALID_SOCKET) { printf("Failed to connect.\n"); return 1; }
            printf("Connected to host!\n");
            playNetworkGame(sock, 0);
        }
        closesocket(sock);
        WSACleanup();
    }
    else {
        printf("Invalid mode.\n");
        return 1;
    }

    return 0;
}
