#include "utils.h"

// 주사위 굴리기
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
    displayDice(dice); // 주사위 출력
}

// 주사위 출력
void displayDice(int dice[]) {
    printf("주사위:\n");
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

int calc_upper_s(int scores[])
{
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        if (scores[i] != -1) {
            sum += scores[i];
        }
    }
    return sum;
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
            return (i + 1) * 4;
            // return i * 4;
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
            //return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3];
            return 1;
    }
    return 0;
}
// Big Straight 점수 계산 : 5개가 연속수일 때
int check_BS(int dice[])
{
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    /*
    for (int i = 0; i < 2; i++)
    {
        int result = counts[i] * counts[i + 1] * counts[i + 2] * counts[i + 3] * counts[i + 4];
        if (result != 0)
            //return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3] && counts[i + 4];
            return 1;
    }
    */

    if (counts[0] == 1 && counts[2] == 1 && counts[3] == 1 && counts[4] == 1 && counts[5] == 1)
        return 1;
    if (counts[2] == 1 && counts[3] == 1 && counts[4] == 1 && counts[5] == 1 && counts[6] == 1)
        return 1;
    return 0;
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
        // printf("Invalid category!\n");
        printf("없는 카테고리입니다!\n");
        break;
    }
}

// 컴퓨터 점수 기록
void record_CS(int scores[], int dice[]) {
    int choice_C = choice_BC(scores, dice);
    int score = calc_score(choice_C, dice);
    scores[choice_C] = score; // 해당 카테고리에 점수 기록
    // printf("Computer scored %d points in category %d.\n", score, choice_C + 1);
    printf("Computer의 %d번째 카테고리에 %d점수를 얻었습니다.\n", choice_C + 1, score);

}


// 게임 중 컴퓨터 점수 출력
void print_CS(int scores[]) {
    printf("Computer의 점수: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 미사용 카테고리
        else printf("[%d] ", scores[i]);
    }
    printf("\n");
}

// 게임 중 플레이어 점수 출력
void print_PS(int scores[]) {
    printf("\nPlayer의 점수: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 미사용 카테고리
        else printf("[%d] ", scores[i]); // 기록된 점수
    }
    printf("\n");
}

// 텍스트 색상 설정 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 주사위 출력 함수 (커서와 선택 상태 표시)
void display_DWC(int dice[], int keep[], int cursor) {
    system("cls"); // 화면 초기화
    // printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");
    printf("방향키(<-, ->)를 사용하여 이동하고, SPACE를 선택을 전환하고, ENTER를 눌러 확인하며, ESC를 종료합니다.\n");

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

    // printf("\n======== Scoreboard ========\n");
    printf("\n======== 스코어보드 ========\n");
    for (int i = 0; i < numCategories; i++) {
        printf("%-20s: %d points\n", categories[i], scores[i]);
    }
    printf("============================\n");
}

// 최종 선택 상태 확인 함수
int select_confirm(int dice[], int keep[]) {
    system("cls");
    // printf("Confirm your selection:\n");
    printf("선택 확인:\n");
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
    // printf("\nPress ENTER to confirm or ESC to go back.\n");
    printf("\nENTER: 계속 ESC: 뒤로가기\n");
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
                printf("주사위를 다시 굴리는 중입나다...\n");
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
