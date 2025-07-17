// main.c
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN            // Windows.h 포함 시 winsock.h 중복을 막음
#include <winsock2.h>                  // 반드시 Windows.h 앞
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "main.h"
#include "graphics.h"

#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE        512

// 네트워크로 주고받을 한 턴의 정보
typedef struct {
    int dice[NUM_DICE];
    int keep[NUM_DICE];
    int category;
    int score;
} TurnData;


//-----------------------------------------------------------------------------
// ▶ UI 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void displayDice(int dice[]) {
    printf("Current dice:\n");
    displayAllDiceHorizontal(dice, NUM_DICE);
    printf("\n");
}

void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++)
        if (!keep[i])
            dice[i] = rand() % 6 + 1;
    displayDice(dice);
}

void display_DWC(int dice[], int keep[], int cursor) {
    system("cls");
    printf("Use ↔ to move, SPACE to toggle, ENTER to confirm, ESC to exit\n");
    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor)      setColor(CURSOR_COLOR);
        else if (keep[i])     setColor(SELECTED_COLOR);
        else                  setColor(DEFAULT_COLOR);
        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR);
    }
    printf("\n");
}

int select_confirm(int dice[], int keep[]) {
    system("cls");
    printf("Confirm selection:\n");
    for (int i = 0; i < NUM_DICE; i++) {
        setColor(keep[i] ? SELECTED_COLOR : DEFAULT_COLOR);
        printf(" %d ", dice[i]);
    }
    setColor(DEFAULT_COLOR);
    printf("\nENTER=OK, ESC=Back\n");
    while (1) {
        int k = _getch();
        if (k == ENTER) return 1;
        if (k == ESC)   return 0;
    }
}

void dice_S(int dice[], int keep[]) {
    int cursor = 0, run = 1;
    while (run) {
        display_DWC(dice, keep, cursor);
        int k = _getch();
        if (k == ARROW && (k = _getch()) == LEFT)   cursor = (cursor + NUM_DICE - 1) % NUM_DICE;
        else if (k == ARROW && k == RIGHT)               cursor = (cursor + 1) % NUM_DICE;
        else if (k == SPACEBAR)                         keep[cursor] ^= 1;
        else if (k == ENTER && select_confirm(dice, keep)) {
            Sleep(500);
            run = 0;
        }
        else if (k == ESC) { Sleep(500); run = 0; }
    }
}

void print_PS(int sc[]) {
    printf("\nPlayer:   ");
    for (int i = 0; i < NUM_CATEGORIES; i++)
        printf(sc[i] == -1 ? "[  ] " : "[%2d] ", sc[i]);
    printf("\n");
}

void print_CS(int sc[]) {
    printf("Computer: ");
    for (int i = 0; i < NUM_CATEGORIES; i++)
        printf(sc[i] == -1 ? "[  ] " : "[%2d] ", sc[i]);
    printf("\n");
}


//-----------------------------------------------------------------------------
// ▶ 점수 계산 함수
int calc_sum(int d[], int n) {
    int s = 0;
    for (int i = 0; i < NUM_DICE; i++)
        if (n == 0 || d[i] == n) s += d[i];
    return s;
}

int check_Y(int d[]) {
    for (int i = 1; i < NUM_DICE; i++) if (d[i] != d[0]) return 0;
    return 1;
}

int check_FOK(int d[]) {
    int cnt[7] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
    for (int v = 1; v <= 6; v++)
        if (cnt[v] >= 4) return v * 4;
    return 0;
}

int check_FH(int d[]) {
    int cnt[7] = { 0 }, two = 0, three = 0;
    for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
    for (int v = 1; v <= 6; v++) {
        if (cnt[v] == 2) two = 1;
        if (cnt[v] == 3) three = 1;
    }
    return two && three;
}

int check_LS(int d[]) {
    int cnt[7] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
    for (int s = 1; s <= 3; s++)
        if (cnt[s] && cnt[s + 1] && cnt[s + 2] && cnt[s + 3])
            return 1;
    return 0;
}

int check_BS(int d[]) {
    int cnt[7] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
    if (cnt[1] && cnt[2] && cnt[3] && cnt[4] && cnt[5]) return 1;
    if (cnt[2] && cnt[3] && cnt[4] && cnt[5] && cnt[6]) return 1;
    return 0;
}

int calc_score(int cat, int d[]) {
    switch (cat) {
    case 0:  return calc_sum(d, 1);
    case 1:  return calc_sum(d, 2);
    case 2:  return calc_sum(d, 3);
    case 3:  return calc_sum(d, 4);
    case 4:  return calc_sum(d, 5);
    case 5:  return calc_sum(d, 6);
    case 6:  return check_Y(d) ? 50 : 0;
    case 7:  return check_FOK(d);
    case 8:  return check_FH(d) ? 25 : 0;
    case 9:  return check_LS(d) ? 30 : 0;
    case 10: return check_BS(d) ? 40 : 0;
    case 11: return calc_sum(d, 0);
    }
    return 0;
}

int choice_BC(int sc[], int d[]) {
    int best = 0, mx = -1;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (sc[i] != -1) continue;
        int v = calc_score(i, d);
        if (v > mx) { mx = v; best = i; }
    }
    return best;
}

void decide_KD(int d[], int keep[], int tc) {
    memset(keep, 0, sizeof(int) * NUM_DICE);
    if (tc <= 5) {
        for (int i = 0; i < NUM_DICE; i++)
            if (d[i] == tc + 1) keep[i] = 1;
    }
    else if (tc == 6 && check_Y(d)) {
        for (int i = 0; i < NUM_DICE; i++) keep[i] = 1;
    }
    else if (tc == 7) {
        int cnt[7] = { 0 };
        for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
        for (int v = 1; v <= 6; v++) {
            if (cnt[v] >= 4) for (int i = 0; i < NUM_DICE; i++)
                if (d[i] == v) keep[i] = 1;
        }
    }
    else if (tc == 8 && check_FH(d)) {
        int cnt[7] = { 0 };
        for (int i = 0; i < NUM_DICE; i++) cnt[d[i]]++;
        for (int v = 1; v <= 6; v++) {
            if (cnt[v] == 2 || cnt[v] == 3)
                for (int i = 0; i < NUM_DICE; i++)
                    if (d[i] == v) keep[i] = 1;
        }
    }
    else if ((tc == 9 && check_LS(d)) || (tc == 10 && check_BS(d))) {
        // straight: keep any part of it
        for (int i = 0; i < NUM_DICE; i++) {
            if ((tc == 9 && d[i] >= 1 && d[i] <= 4) ||
                (tc == 10 && (d[i] <= 5 && d[i] >= 1)) ||
                (tc == 10 && (d[i] >= 2 && d[i] <= 6)))
                keep[i] = 1;
        }
    }
    else if (tc == 11) {
        for (int i = 0; i < NUM_DICE; i++) keep[i] = 1;
    }
}

void record_CS(int sc[], int d[]) {
    int c = choice_BC(sc, d);
    sc[c] = calc_score(c, d);
    printf("Computer scored %d in category %d\n", sc[c], c + 1);
}


//-----------------------------------------------------------------------------
// ▶ 네트워크 헬퍼
static int recvAll(SOCKET s, char* buf, int len) {
    int r, pos = 0;
    while (pos < len) {
        r = recv(s, buf + pos, len - pos, 0);
        if (r <= 0) return 0;
        pos += r;
    }
    return 1;
}

void sendTurnData(SOCKET s, TurnData* td) {
    send(s, (char*)td, sizeof(*td), 0);
}

int recvTurnData(SOCKET s, TurnData* td) {
    return recvAll(s, (char*)td, sizeof(*td));
}


//-----------------------------------------------------------------------------
// ▶ 한 로컬 턴 실행 (내 턴)
void playLocalTurn(int sc[], TurnData* td) {
    int dice[NUM_DICE], keep[NUM_DICE] = { 0 };

    // 최대 3번
    for (int r = 0; r < 3; r++) {
        rollDice(dice, keep);
        printf("Roll %d complete.\n", r + 1);
        if (r < 2) {
            printf("Select dice to keep...\n"); dice_S(dice, keep);
        }
    }

    // 카테고리 선택
    int done = 0;
    while (!done) {
        displayDice(dice); print_PS(sc);
        printf("Choose category (1-12): ");
        int c; scanf("%d", &c); c--;
        if (c >= 0 && c < NUM_CATEGORIES && sc[c] == -1) {
            sc[c] = calc_score(c, dice);
            printf("You scored %d\n", sc[c]);
            td->category = c;
            td->score = sc[c];
            done = 1;
        }
        else {
            printf("Invalid / used category.\n");
        }
    }

    // 턴 데이터 기록
    memcpy(td->dice, dice, sizeof(dice));
    memcpy(td->keep, keep, sizeof(keep));
}


//-----------------------------------------------------------------------------
// ▶ 상대 턴 데이터 수신 후 화면에만 표시
void processRemoteTurn(int sc[], TurnData* td) {
    system("cls");
    printf("=== Opponent's Turn ===\n");
    display_DWC(td->dice, td->keep, -1);
    printf("Opponent scored %d in category %d\n",
        td->score, td->category + 1);
    sc[td->category] = td->score;
    printf("Press any key to continue...\n");
    _getch();
}

//-----------------------------------------------------------------------------
// ▶ 네트워크 모드 게임 루프 (12 턴)
void playNetworkGame(SOCKET s, int isHost) {
    int mySc[NUM_CATEGORIES], opSc[NUM_CATEGORIES];
    for (int i = 0; i < NUM_CATEGORIES; i++)
        mySc[i] = opSc[i] = -1;

    for (int turn = 0; turn < NUM_CATEGORIES; turn++) {
        TurnData td;
        if (isHost) {
            // 1) 내 턴
            playLocalTurn(mySc, &td);
            sendTurnData(s, &td);
            // 2) 상대 턴 수신
            if (!recvTurnData(s, &td)) { printf("Connection lost.\n"); return; }
            processRemoteTurn(opSc, &td);
        }
        else {
            // 1) 상대 턴 수신
            if (!recvTurnData(s, &td)) { printf("Connection lost.\n"); return; }
            processRemoteTurn(opSc, &td);
            // 2) 내 턴
            playLocalTurn(mySc, &td);
            sendTurnData(s, &td);
        }
    }

    // 최종 점수 합산
    int sumMy = 0, sumOp = 0;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (mySc[i] != -1) sumMy += mySc[i];
        if (opSc[i] != -1) sumOp += opSc[i];
    }
    printf("\n=== Final Results ===\nYou: %d\nOpponent: %d\n", sumMy, sumOp);
    if (sumMy > sumOp)  printf("You Win!\n");
    else if (sumMy < sumOp)  printf("You Lose...\n");
    else                     printf("Draw.\n");
}


//-----------------------------------------------------------------------------
// ▶ 소켓 준비: Host, Client
SOCKET setupHost(const char* port) {
    WSADATA wsa;    SOCKET ls, cs;
    struct addrinfo hint = { 0 }, * res;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hint, &res);
    ls = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(ls, res->ai_addr, (int)res->ai_addrlen);
    listen(ls, 1);

    printf("Waiting for opponent on port %s...\n", port);
    cs = accept(ls, NULL, NULL);
    closesocket(ls);
    return cs;
}

SOCKET setupClient(const char* ip, const char* port) {
    WSADATA wsa;    SOCKET s;
    struct addrinfo hint = { 0 }, * res;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;

    getaddrinfo(ip, port, &hint, &res);
    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(s, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR)
        return INVALID_SOCKET;
    return s;
}


//-----------------------------------------------------------------------------
// ▶ main()
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
        // … 여기에 기존 싱글 플레이 전체 루프 복사 …
    }
    else if (mode == 2) {
        // … 여기에 기존 VS Computer 전체 루프 복사 …
    }
    else if (mode == 3) {
        int nc;
        char ip[128], port[6];
        SOCKET sock;

        printf("1) Host game   2) Join game\nSelect: ");
        scanf("%d", &nc);
        printf("Enter port: ");
        scanf("%5s", port);

        if (nc == 1) {
            sock = setupHost(port);
            if (sock == INVALID_SOCKET) { printf("Host failed.\n"); return 1; }
            printf("Opponent connected!\n");
            playNetworkGame(sock, 1);
        }
        else {
            printf("Enter host IP: ");
            scanf("%127s", ip);
            sock = setupClient(ip, port);
            if (sock == INVALID_SOCKET) { printf("Connect failed.\n"); return 1; }
            printf("Connected!\n");
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
