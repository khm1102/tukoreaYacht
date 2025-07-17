#define _CRT_SECURE_NO_WARNINGS
#include "main.h"   
// UI �Լ�

// �ؽ�Ʈ ���� ���� �Լ�
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// �ֻ��� ��� �Լ� (Ŀ���� ���� ���� ǥ��)
void display_DWC(int dice[], int keep[], int cursor) {
    system("cls"); // ȭ�� �ʱ�ȭ
    printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor) {
            setColor(CURSOR_COLOR); // Ŀ���� ��ġ�� �ֻ����� �Ķ���
        }
        else if (keep[i]) {
            setColor(SELECTED_COLOR); // ���õ� �ֻ����� ���
        }
        else {
            setColor(DEFAULT_COLOR); // �⺻ ����
        }

        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR); // ���� �ʱ�ȭ
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

// ���� ���� ���� Ȯ�� �Լ�
int select_confirm(int dice[], int keep[]) {
    system("cls");
    printf("Confirm your selection:\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i]) {
            setColor(SELECTED_COLOR); // ���õ� �ֻ����� ���
        }
        else {
            setColor(DEFAULT_COLOR); // �⺻ ����
        }
        printf(" %d ", dice[i]);
    }
    setColor(DEFAULT_COLOR); // ���� �ʱ�ȭ
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


// ȭ��ǥ Ű�� �ֻ����� �����ϰų� ����/���� ó��
void dice_S(int dice[], int keep[]) {
    int cursor = 0; // Ŀ�� �ʱ� ��ġ
    int running = 1;

    while (running) {
        display_DWC(dice, keep, cursor); // �ֻ��� ���

        int key = _getch(); // Ű �Է� ó��
        if (key == ARROW) {
            key = _getch();
            if (key == LEFT) {
                cursor = (cursor == 0) ? NUM_DICE - 1 : cursor - 1; // ���� �̵�
            }
            else if (key == RIGHT) {
                cursor = (cursor == NUM_DICE - 1) ? 0 : cursor + 1; // ������ �̵�
            }
        }
        else if (key == SPACEBAR) {
            keep[cursor] = !keep[cursor]; // ���� ���� ���
        }
        else if (key == ENTER) {
            if (select_confirm(dice, keep)) {
                printf("Selection confirmed. Re-rolling dice...\n");
                Sleep(1000); // 1�� ��� �� ����
                running = 0;
            }
        }
        else if (key == ESC) {
            printf("Exiting dice selection...\n");
            Sleep(1000); // 1�� ��� �� ����
            running = 0;
        }
    }
}

// ���� �Լ�
int main() {
    displayWelcomeScreen();   // ���� ȭ���� ǥ��
    int dice[NUM_DICE];       // ���� �ֻ��� ��
    // int keep[NUM_DICE] = { 0 }; // ������ �ֻ��� ǥ��
    int PS[NUM_CATEGORIES];
    int CS[NUM_CATEGORIES];
    int category, roll, i, mode;
    int* keep;
    char ip[20];

    keep = (int*)malloc(NUM_DICE * sizeof(int));

    // ���� �迭 �ʱ�ȭ
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        PS[i] = -1;    // -1�� �ʱ�ȭ
        CS[i] = -1;  // -1�� �ʱ�ȭ
    }

    srand(time(0)); // ���� �õ� ����

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

        // �÷��̾� ��
        if (mode == 2) {
            printf("\nYour turn!\n");
        }

        for (i = 0; i < NUM_DICE; i++) {
            keep[i] = 0;
        }

        // �ֻ��� ������ �ִ� 3��
        for (roll = 0; roll < 3; roll++) {
            rollDice(dice, keep);
            printf("\nPress any key to continue...\n");
            _getch(); // ����ڰ� Ű�� ���� ������ ���
            printf("\nRoll %d: \n", roll + 1);

            if (roll < 2) {
                printf("You can choose which dice to keep or re-roll.\n");
                dice_S(dice, keep); // ȭ��ǥ Ű�� ����� �ֻ��� ����
            }
        }

        int chosen = 1;
        while (chosen) {
            displayDice(dice);
            print_PS(PS); // �÷��̾� ���� ���
            printf("Choose a category (1:Ones, 2:Twos, 3:Threes, 4:Fours, 5:Fives, 6:Sixes, 7:Yacht, 8:Four of a Kind, 9:Full House, 10:Little Straight, 11:Big Straight, 12:Choice): ");
            scanf("%d", &category);
            category--; // �迭 �ε��� ���߱�

            if (PS[category] == -1) { // ���� ���õ��� ���� ī�װ�
                PS[category] = calc_score(category, dice);
                printf("You scored %d points in this category.\n", PS[category]);
                chosen = 0;
            }
            else {
                printf("Category already chosen, try another category.\n");
            }
        }

        // ��ǻ�� ��
        if (mode == 2) {
            printf("\nComputer's turn!\n");

            // ��ǻ�� �ֻ��� ���� ���� �ʱ�ȭ
            for (i = 0; i < NUM_DICE; i++) {
                keep[i] = 0;
            }

            // ��ǻ�� �ֻ��� ������ (3ȸ)
            for (roll = 0; roll < 3; roll++) {
                rollDice(dice, keep);
                printf("\nComputer's Roll %d: \n", roll + 1);


                // ù ��°�� �� ��° ���������� �ֻ��� ���� ���� ����
                if (roll < 2) {
                    int targetCategory = choice_BC(CS, dice);
                    if (targetCategory >= 0 && targetCategory < NUM_CATEGORIES) {
                        decide_KD(dice, keep, targetCategory);
                    }
                }
            }

            displayAllDiceHorizontal(dice, NUM_DICE);

            // ��ǻ�� ���� ���
            record_CS(CS, dice);

            // ��ǻ�� ���� ���
            print_CS(CS);
        }

    }

    // ���� ���� ���
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


// �ֻ��� ������
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
    displayDice(dice); // �ֻ��� ���
}

// �ӽ÷� �ּ� ����


// �ֻ��� ���
void displayDice(int dice[]) {
    printf("Current dice:\n");
    displayAllDiceHorizontal(dice, NUM_DICE); // �ֻ��� �迭�� �ֻ��� ���� ����
    printf("\n");
}


// ���� ��� �Լ�
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
// Ư�� ���� �ջ�
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
// Yacht ���� ��� : ��� ���� ���� ��
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
// Four of a Kind ���� ��� : 4���� ���� ���� ��
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
// Full House ���� ��� : 3��,2��
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
// Little Straight ���� ��� : 4���� ���Ӽ��� ��
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
// Big Straight ���� ��� : 5���� ���Ӽ��� ��
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

// ��ǻ�� ���� ī�װ� ����
int choice_BC(int scores[], int dice[]) {
    int max_S = -1;
    int best_C = -1;

    // ���� ���� ������ ����� ī�װ� ����
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] != -1) continue; // �̹� ���� ī�װ� �ǳʶٱ�
        int cur_S = calc_score(i, dice);
        if (cur_S > max_S) {
            max_S = cur_S;
            best_C = i;
        }
    }

    // ��ȿ�� ī�װ��� ������ -1 ��ȯ
    return best_C;
}


void decide_KD(int dice[], int keep[], int target_C) {
    // targetCategory�� ���� �ֻ��� ���� ������ ����
    switch (target_C) {
    case 0: // Ones
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 1) {
                keep[i] = 1; // 1�� ���� �ֻ����� ����
            }
            else {
                keep[i] = 0; // �ٸ� �ֻ����� �ٽ� ����
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
                keep[i] = 1; // ��� �����ϸ� ��� ����
            }
        }
        break;

    case 7: // Four of a Kind
        if (check_FOK(dice)) {
            // 4�� �̻� ������ ���� ���� ���, �ش� �� ����
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
            // 3���� 2�� ����
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
            // ���ӵ� 4�� ���� ����
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
            // ���ӵ� 5�� ���� ����
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

// ��ǻ�� ���� ���
void record_CS(int scores[], int dice[]) {
    int choice_C = choice_BC(scores, dice);
    int score = calc_score(choice_C, dice);
    scores[choice_C] = score; // �ش� ī�װ��� ���� ���
    printf("Computer scored %d points in category %d.\n", score, choice_C + 1);
}


// ���� �� ��ǻ�� ���� ���
void print_CS(int scores[]) {
    printf("Computer scores: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // �̻�� ī�װ�
        else printf("[%d] ", scores[i]);
    }
    printf("\n");
}

// ���� �� �÷��̾� ���� ���
void print_PS(int scores[]) {
    printf("\nPlayer scores: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // �̻�� ī�װ�
        else printf("[%d] ", scores[i]); // ��ϵ� ����
    }
    printf("\n");
}

