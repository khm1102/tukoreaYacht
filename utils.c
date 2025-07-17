#include "utils.h"

// �ֻ��� ������
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
    displayDice(dice); // �ֻ��� ���
}

// �ֻ��� ���
void displayDice(int dice[]) {
    printf("�ֻ���:\n");
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
        return calc_sum(dice, 0); // Choice
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
        return check_Y(dice) ? 50 : 0; // Yacht
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
            return (i + 1) * 4;
            // return i * 4;
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
            //return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3];
            return 1;
    }
    return 0;
}
// Big Straight ���� ��� : 5���� ���Ӽ��� ��
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
        // printf("Invalid category!\n");
        printf("���� ī�װ��Դϴ�!\n");
        break;
    }
}

// ��ǻ�� ���� ���
void record_CS(int scores[], int dice[]) {
    int choice_C = choice_BC(scores, dice);
    int score = calc_score(choice_C, dice);
    scores[choice_C] = score; // �ش� ī�װ��� ���� ���
    // printf("Computer scored %d points in category %d.\n", score, choice_C + 1);
    printf("Computer�� %d��° ī�װ��� %d������ ������ϴ�.\n", choice_C + 1, score);

}

// ���� �� �÷��̾�� ��ǻ�� ���� ���
void print_MS(int ply[], int com[], int* localData, int* comData) {
    int Ptotal = 0, Ctotal = 0;

    for (int i = 0; i < 6; i++) {
        Ptotal += ply[i];
        Ctotal += com[i];
    }

    printf("%15s����������������������������������������������\n", "������        ");
    printf("%20s��%10s��%10s��\n", " �� �� ������������", "", "");
    printf("%19s��%10s��%10s��\n", " �� �������� �� ��", "", "");
    printf("%12s��%10s��%10s��\n", "12/12", "player", "computer");
    printf("��������������������������%10s��%10s��\n", "", "");
    printf("��%11s��%10s��%10s��\n", "Categories", "", "");
    printf("����������������������������������������������������������������������\n");

    printf("��%11s��%10d��%10d��\n", "Aces", ply[0], com[0]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Deuces", ply[1], com[1]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Threes", ply[2], com[2]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Fours", ply[3], com[3]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Fives", ply[4], com[4]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Sixes", ply[5], com[5]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%7d%s��%7d%s��\n", "Subtotal", Ptotal, "/63", Ctotal, "/63");
    printf("��***********��**********��**********��\n");

    if (Ptotal == 63) {
        Ptotal += 35;
    }
    if (Ctotal == 63) {
        Ctotal += 35;
    }

    printf("��%11s��%10d��%10d��\n", "+35 Bonus", Ptotal, Ctotal);
    printf("����������������������������������������������������������������������\n");

    printf("Bonus if 1 - 6 are over 63 points\n");

    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Choice", ply[6], com[6]);
    printf("����������������������������������������������������������������������\n");
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "4 of a Kind", ply[7], com[7]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Full House", ply[8], com[8]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "S. Straight", ply[9], com[9]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "L. Straight", ply[10], com[10]);
    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Yacht", ply[11], com[11]);
    printf("����������������������������������������������������������������������\n");

    for (int i = 6; i < 12; i++) {
        Ptotal += ply[i];
        Ctotal += com[i];
    }

    *localData = Ptotal;
    *comData = Ctotal;

    printf("����������������������������������������������������������������������\n");
    printf("��%11s��%10d��%10d��\n", "Total", Ptotal, Ctotal);
    printf("����������������������������������������������������������������������\n");

    
    
}


// �� ���� �� ���Ƽ� �ּ�
// ���� �� ��ǻ�� ���� ���
//void print_CS(int scores[]) {
//    printf("Computer�� ����: ");
//    for (int i = 0; i < NUM_CATEGORIES; i++) {
//        if (scores[i] == -1) printf("[ ] "); // �̻�� ī�װ�
//        else printf("[%d] ", scores[i]);
//    }
//    printf("\n");
//}

// ���� �� �÷��̾� ���� ���
void print_PS(int scores[]) {
    printf("\nPlayer�� ����: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // �̻�� ī�װ�
        else printf("[%d] ", scores[i]); // ��ϵ� ����
    }
    printf("\n");
}

// �ؽ�Ʈ ���� ���� �Լ�
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// �ֻ��� ��� �Լ� (Ŀ���� ���� ���� ǥ��)
void display_DWC(int dice[], int keep[], int cursor) {
    system("cls"); // ȭ�� �ʱ�ȭ
    // printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");
    printf("����Ű(<-, ->)�� ����Ͽ� �̵��ϰ�, SPACE�� ������ ��ȯ�ϰ�, ENTER�� ���� Ȯ���ϸ�, ESC�� �����մϴ�.\n");

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

// ���ھ�� ��
void scoreBoard(int ply[]) {
    int total = 0;
    /*
    for (int i = 0; i < 6; i++) {
        if (ply[i] == -1) {
            ply[i]
        }

        total += ply[i];
    }
    */
    for (int i = 0; i < 6; i++) {
        total += ply[i];
    }
    printf("%15s������������������������\n", "������        ");
    printf("%20s��%10s��\n", " �� �� ������������", "");
    printf("%19s��%10s��\n", " �� �������� �� ��", "");
    printf("%12s��%10s��\n", "12/12", "player");
    printf("��������������������������%10s��\n", "");
    printf("��%11s��%10s��\n", "Categories", "");
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Aces", ply[0]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Deuces", ply[1]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Threes", ply[2]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Fours", ply[3]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Fives", ply[4]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Sixes", ply[5]);
    printf("������������������������������������������������\n");
    printf("Bonus if 1 - 6 are over 63 points\n");
    printf("������������������������������������������������\n");
    printf("��%11s��%7d%s��\n", "Subtotal", total, "/63");
    printf("��***********��**********��\n");

    if (total == 63) {
        total += 35;
    }

    printf("��%11s��%10d��\n", "+35 Bonus", total);
    printf("������������������������������������������������\n");

    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Choice", ply[6]);
    printf("������������������������������������������������\n");
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "4 of a Kind", ply[7]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Full House", ply[8]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "S. Straight", ply[9]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "L. Straight", ply[10]);
    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Yacht", ply[11]);
    printf("������������������������������������������������\n");

    for (int i = 6; i < 12; i++) {
        total += ply[i];
    }

    printf("������������������������������������������������\n");
    printf("��%11s��%10d��\n", "Total", total);
    printf("������������������������������������������������\n\n");

}

void display_SB(int scores[], int numCategories) {
    scoreBoard(scores);
}

// ���� ���� ���� Ȯ�� �Լ�
int select_confirm(int dice[], int keep[]) {
    system("cls");
    // printf("Confirm your selection:\n");
    printf("���� Ȯ��:\n");
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
    // printf("\nPress ENTER to confirm or ESC to go back.\n");
    printf("\nENTER: ��� ESC: �ڷΰ���\n");
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
                printf("�ֻ����� �ٽ� ������ ���Դϴ�...\n");
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
