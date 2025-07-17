#define _CRT_SECURE_NO_WARNINGS
#include "main.h"   
// UI 함수

// 텍스트 색상 설정 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 주사위 출력 함수 (커서와 선택 상태 표시)
void display_DWC(int dice[], int keep[], int cursor) {
    system("cls"); // 화면 초기화
    printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor) {
            setColor(CURSOR_COLOR); // 커서에 위치한 주사위는 파란색
        }
        else if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위는 녹색
        }
        else {
            setColor(DEFAULT_COLOR); // 기본 색상
        }

        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR); // 색상 초기화
    }
    printf("\n");
}

void display_SB(int scores[], int numCategories) {
    const char* categories[] = {
        "1: Ones", "2: Twos", "3: Threes", "4: Fours", "5: Fives", "6: Sixes",
        "7: Yacht", "8: Four of a Kind", "9: Full House",
        "10: Little Straight", "11: Big Straight", "12: Choice"
    };

    printf("\n======== Scoreboard ========\n");
    for (int i = 0; i < numCategories; i++) {
        printf("%-20s: %d points\n", categories[i], scores[i]);
    }
    printf("============================\n");
}

// 최종 선택 상태 확인 함수
int select_confirm(int dice[], int keep[]) {
    system("cls");
    printf("Confirm your selection:\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위는 녹색
        }
        else {
            setColor(DEFAULT_COLOR); // 기본 색상
        }
        printf(" %d ", dice[i]);
    }
    setColor(DEFAULT_COLOR); // 색상 초기화
    printf("\nPress ENTER to confirm or ESC to go back.\n");

    while (1) {
        int key = _getch();
        if (key == ENTER) {
            return 1;
        }
        else if (key == ESC) {
            return 0;
        }
    }
}


// 화살표 키로 주사위를 선택하거나 유지/해제 처리
void dice_S(int dice[], int keep[]) {
    int cursor = 0; // 커서 초기 위치
    int running = 1;

    while (running) {
        display_DWC(dice, keep, cursor); // 주사위 출력

        int key = _getch(); // 키 입력 처리
        if (key == ARROW) {
            key = _getch();
            if (key == LEFT) {
                cursor = (cursor == 0) ? NUM_DICE - 1 : cursor - 1; // 왼쪽 이동
            }
            else if (key == RIGHT) {
                cursor = (cursor == NUM_DICE - 1) ? 0 : cursor + 1; // 오른쪽 이동
            }
        }
        else if (key == SPACEBAR) {
            keep[cursor] = !keep[cursor]; // 선택 상태 토글
        }
        else if (key == ENTER) {
            if (select_confirm(dice, keep)) {
                printf("Selection confirmed. Re-rolling dice...\n");
                Sleep(1000); // 1초 대기 후 종료
                running = 0;
            }
        }
        else if (key == ESC) {
            printf("Exiting dice selection...\n");
            Sleep(1000); // 1초 대기 후 종료
            running = 0;
        }
    }
}

// 메인 함수
int main() {
    displayWelcomeScreen();   // 시작 화면을 표시
    int dice[NUM_DICE];       // 현재 주사위 눈
    // int keep[NUM_DICE] = { 0 }; // 유지할 주사위 표시
    int PS[NUM_CATEGORIES];
    int CS[NUM_CATEGORIES];
    int category, roll, i, mode;
    int* keep;
    char ip[20];

    keep = (int*)malloc(NUM_DICE * sizeof(int));

    // 점수 배열 초기화
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        PS[i] = -1;    // -1로 초기화
        CS[i] = -1;  // -1로 초기화
    }

    srand(time(0)); // 랜덤 시드 설정

    printf("Welcome to Yacht Dice!\n");
    printf("\nEnter a number to select a mode (1: Single play, 2: VS Computer, 3: IP): ");
    scanf("%d", &mode);

    // Easter Egg!
    if (mode == 123456) {
        system("cls");
        printf("================================\n");
        printf("  _     _     _     _     _     \n");
        printf(" | |   | |   | |   | |   | |    \n");
        printf(" | |   | |   | |   | |   | |    \n");
        printf(" | |   | |   | |   | |   | |    \n");
        printf(" |_____|_____|_____|_____|____| \n");
        printf("          YACHT DICE           \n");
        printf("================================\n");
        Sleep(1000);
        Sleep(2000);
        printf("\n^_^\n");
        Sleep(2000);

        return 0;
    }

    for (int turn = 0; turn < NUM_CATEGORIES; turn++) {
        printf("\n--- Turn %d ---\n", turn + 1);

        // 플레이어 턴
        if (mode == 2) {
            printf("\nYour turn!\n");
        }

        for (i = 0; i < NUM_DICE; i++) {
            keep[i] = 0;
        }

        // 주사위 굴리기 최대 3번
        for (roll = 0; roll < 3; roll++) {
            rollDice(dice, keep);
            printf("\nPress any key to continue...\n");
            _getch(); // 사용자가 키를 누를 때까지 대기
            printf("\nRoll %d: \n", roll + 1);

            if (roll < 2) {
                printf("You can choose which dice to keep or re-roll.\n");
                dice_S(dice, keep); // 화살표 키를 사용해 주사위 선택
            }
        }

        int chosen = 1;
        while (chosen) {
            displayDice(dice);
            print_PS(PS); // 플레이어 점수 출력
            printf("Choose a category (1:Ones, 2:Twos, 3:Threes, 4:Fours, 5:Fives, 6:Sixes, 7:Yacht, 8:Four of a Kind, 9:Full House, 10:Little Straight, 11:Big Straight, 12:Choice): ");
            scanf("%d", &category);
            category--; // 배열 인덱스 맞추기

            if (PS[category] == -1) { // 아직 선택되지 않은 카테고리
                PS[category] = calc_score(category, dice);
                printf("You scored %d points in this category.\n", PS[category]);
                chosen = 0;
            }
            else {
                printf("Category already chosen, try another category.\n");
            }
        }

        // 컴퓨터 턴
        if (mode == 2) {
            printf("\nComputer's turn!\n");

            // 컴퓨터 주사위 유지 여부 초기화
            for (i = 0; i < NUM_DICE; i++) {
                keep[i] = 0;
            }

            // 컴퓨터 주사위 굴리기 (3회)
            for (roll = 0; roll < 3; roll++) {
                rollDice(dice, keep);
                printf("\nComputer's Roll %d: \n", roll + 1);


                // 첫 번째와 두 번째 굴림에서는 주사위 선택 로직 수행
                if (roll < 2) {
                    int targetCategory = choice_BC(CS, dice);
                    if (targetCategory >= 0 && targetCategory < NUM_CATEGORIES) {
                        decide_KD(dice, keep, targetCategory);
                    }
                }
            }

            displayAllDiceHorizontal(dice, NUM_DICE);

            // 컴퓨터 점수 기록
            record_CS(CS, dice);

            // 컴퓨터 점수 출력
            print_CS(CS);
        }

    }

    // 최종 점수 출력
    printf("\nFinal Scores:\n");
    print_PS(PS);
    if (mode == 2) {
        print_CS(CS);

        int total_PS = 0, total_CS = 0;
        for (i = 0; i < NUM_CATEGORIES; i++) {
            if (PS[i] != -1) total_PS += PS[i];
            if (CS[i] != -1) total_CS += CS[i];
        }

        printf("Your total score: %d\n", total_PS);
        printf("Computer's total score: %d\n", total_CS);

        if (total_PS > total_CS) {
            printf("You Won!\n");
        }
        else if (total_PS < total_CS) {
            printf("You Lost!\n");
        }
        else {
            printf("It's a Draw!\n");
        }
    }
    else {
        int total_PS_2 = 0;
        for (i = 0; i < NUM_CATEGORIES; i++) {
            if (PS[i] != -1) total_PS_2 += PS[i];
        }
        printf("Your total score: %d\n", total_PS_2);
    }

    printf("\nPlayer");
    display_SB(PS, NUM_CATEGORIES);
    Sleep(10);

    if (mode == 2) {
        printf("\nComputer");
        display_SB(CS, NUM_CATEGORIES);
    }

    if (mode == 3) {
        printf("IP: ");
        scanf("%s", ip);
        printf("IP: %s", ip);
    }
    return 0;
}


// 주사위 굴리기
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
    displayDice(dice); // 주사위 출력
}

// 임시로 주석 해제


// 주사위 출력
void displayDice(int dice[]) {
    printf("Current dice:\n");
    displayAllDiceHorizontal(dice, NUM_DICE); // 주사위 배열과 주사위 개수 전달
    printf("\n");
}


// 점수 계산 함수
int calc_score(int category, int dice[])
{
    switch (category)
    {
    case 0:
        return calc_sum(dice, 1); // Ones
    case 1:
        return calc_sum(dice, 2); // Twos
    case 2:
        return calc_sum(dice, 3); // Threes
    case 3:
        return calc_sum(dice, 4); // Fours
    case 4:
        return calc_sum(dice, 5); // Fives
    case 5:
        return calc_sum(dice, 6); // Sixes
    case 6:
        return check_Y(dice) ? 50 : 0; // Yacht
    case 7:
        return check_FOK(dice); // Four of a Kind
    case 8:
        return check_FH(dice) ? 25 : 0; // Full House
    case 9:
    {
        return check_LS(dice) ? 30 : 0; // Little Straight
    }
    case 10:
    {
        return check_BS(dice) ? 40 : 0; // Big Straight
    }
    case 11:
    {
        return calc_sum(dice, 0); // Choice
    }
    default:
        return 0;
    }
}
// 특정 숫자 합산
int calc_sum(int dice[], int num)
{
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++)
    {
        if (num == 0 || dice[i] == num)
        {
            sum += dice[i];
        }
    }
    return sum;
}
// Yacht 점수 계산 : 모든 눈이 같을 때
int check_Y(int dice[])
{
    for (int i = 1; i < NUM_DICE; i++)
    {
        if (dice[i] != dice[0])
        {
            return 0;
        }
    }
    return 1;
}
// Four of a Kind 점수 계산 : 4개의 눈이 같을 때
int check_FOK(int dice[])
{
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++)
    {
        if (counts[i] >= 4)
        {
            return i * 4;
        }
    }
    return 0;
}
// Full House 점수 계산 : 3개,2개
int check_FH(int dice[])
{
    int counts[6] = { 0 };
    int two = 0, three = 0;
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++)
    {
        if (counts[i] == 3)
            three = 1;
        if (counts[i] == 2)
            two = 1;
    }
    return two && three;
}
// Little Straight 점수 계산 : 4개가 연속수일 때
int check_LS(int dice[])
{
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 4; i++)
    {
        int result = counts[i] * counts[i + 1] * counts[i + 2] * counts[i + 3];
        if (result != 0)
            return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3];
    }
}
// Big Straight 점수 계산 : 5개가 연속수일 때
int check_BS(int dice[])
{
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 2; i++)
    {
        int result = counts[i] * counts[i + 1] * counts[i + 2] * counts[i + 3] * counts[i + 4];
        if (result != 0)
            return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3] && counts[i + 4];
    }
}

// 컴퓨터 최적 카테고리 선택
int choice_BC(int scores[], int dice[]) {
    int max_S = -1;
    int best_C = -1;

    // 가장 높은 점수를 기록할 카테고리 선택
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] != -1) continue; // 이미 사용된 카테고리 건너뛰기
        int cur_S = calc_score(i, dice);
        if (cur_S > max_S) {
            max_S = cur_S;
            best_C = i;
        }
    }

    // 유효한 카테고리가 없으면 -1 반환
    return best_C;
}


void decide_KD(int dice[], int keep[], int target_C) {
    // targetCategory에 따른 주사위 유지 로직을 결정
    switch (target_C) {
    case 0: // Ones
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 1) {
                keep[i] = 1; // 1이 나온 주사위는 유지
            }
            else {
                keep[i] = 0; // 다른 주사위는 다시 굴림
            }
        }
        break;

    case 1: // Twos
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 2) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 2: // Threes
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 3) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 3: // Fours
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 4) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 4: // Fives
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 5) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 5: // Sixes
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 6) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 6: // Yacht
        if (check_Y(dice)) {
            for (int i = 0; i < NUM_DICE; i++) {
                keep[i] = 1; // 모두 동일하면 모두 유지
            }
        }
        break;

    case 7: // Four of a Kind
        if (check_FOK(dice)) {
            // 4개 이상 동일한 눈이 있을 경우, 해당 눈 유지
            int counts[6] = { 0 };
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i] - 1]++;
            }
            for (int i = 0; i < 6; i++) {
                if (counts[i] >= 4) {
                    for (int j = 0; j < NUM_DICE; j++) {
                        if (dice[j] == i + 1) {
                            keep[j] = 1;
                        }
                    }
                }
            }
        }
        break;

    case 8: // Full House
        if (check_FH(dice)) {
            // 3개와 2개 유지
            int counts[6] = { 0 };
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i] - 1]++;
            }
            for (int i = 0; i < 6; i++) {
                if (counts[i] == 3 || counts[i] == 2) {
                    for (int j = 0; j < NUM_DICE; j++) {
                        if (dice[j] == i + 1) {
                            keep[j] = 1;
                        }
                    }
                }
            }
        }
        break;

    case 9: // Little Straight
        if (check_LS(dice)) {
            // 연속된 4개 숫자 유지
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1 || dice[i] == 2 || dice[i] == 3 || dice[i] == 4) {
                    keep[i] = 1;
                }
                if (dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5) {
                    keep[i] = 1;
                }
                if (dice[i] == 3 || dice[i] == 4 || dice[i] == 5 || dice[i] == 6) {
                    keep[i] = 1;
                }
            }
        }
        break;

    case 10: // Big Straight
        if (check_BS(dice)) {
            // 연속된 5개 숫자 유지
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1 || dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5) {
                    keep[i] = 1;
                }
                if (dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5 || dice[i] == 6) {
                    keep[i] = 1;
                }
            }
        }
        break;

    case 11: // Choice
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 1;
        }
        break;

    default:
        printf("Invalid category!\n");
        break;
    }
}

// 컴퓨터 점수 기록
void record_CS(int scores[], int dice[]) {
    int choice_C = choice_BC(scores, dice);
    int score = calc_score(choice_C, dice);
    scores[choice_C] = score; // 해당 카테고리에 점수 기록
    printf("Computer scored %d points in category %d.\n", score, choice_C + 1);
}


// 게임 중 컴퓨터 점수 출력
void print_CS(int scores[]) {
    printf("Computer scores: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 미사용 카테고리
        else printf("[%d] ", scores[i]);
    }
    printf("\n");
}

// 게임 중 플레이어 점수 출력
void print_PS(int scores[]) {
    printf("\nPlayer scores: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 미사용 카테고리
        else printf("[%d] ", scores[i]); // 기록된 점수
    }
    printf("\n");
}

