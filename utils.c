#include "utils.h"

// 輿餌嬪 掉葬晦
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
    displayDice(dice); // 輿餌嬪 轎溘
}

// 輿餌嬪 轎溘
void displayDice(int dice[]) {
    printf("輿餌嬪:\n");
    displayAllDiceHorizontal(dice, NUM_DICE); // 輿餌嬪 寡翮婁 輿餌嬪 偃熱 瞪殖
    printf("\n");
}

// 薄熱 啗骯 л熱
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

// か薑 璋濠 м骯
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
// Yacht 薄熱 啗骯 : 賅萇 換檜 偽擊 陽
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
// Four of a Kind 薄熱 啗骯 : 4偃曖 換檜 偽擊 陽
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
// Full House 薄熱 啗骯 : 3偃,2偃
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
// Little Straight 薄熱 啗骯 : 4偃陛 翱樓熱橾 陽
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
// Big Straight 薄熱 啗骯 : 5偃陛 翱樓熱橾 陽
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

// 闡У攪 譆瞳 蘋纔堅葬 摹鷗
int choice_BC(int scores[], int dice[]) {
    int max_S = -1;
    int best_C = -1;

    // 陛濰 堪擎 薄熱蒂 晦煙й 蘋纔堅葬 摹鷗
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] != -1) continue; // 檜嘐 餌辨脹 蘋纔堅葬 勒傘嗨晦
        int cur_S = calc_score(i, dice);
        if (cur_S > max_S) {
            max_S = cur_S;
            best_C = i;
        }
    }

    // 嶸�褲� 蘋纔堅葬陛 橈戲賊 -1 奩��
    return best_C;
}


void decide_KD(int dice[], int keep[], int target_C) {
    // targetCategory縑 評艇 輿餌嬪 嶸雖 煎霜擊 唸薑
    switch (target_C) {
    case 0: // Ones
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 1) {
                keep[i] = 1; // 1檜 釭螞 輿餌嬪朝 嶸雖
            }
            else {
                keep[i] = 0; // 棻艇 輿餌嬪朝 棻衛 掉葡
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
                keep[i] = 1; // 賅舒 翕橾ж賊 賅舒 嶸雖
            }
        }
        break;

    case 7: // Four of a Kind
        if (check_FOK(dice)) {
            // 4偃 檜鼻 翕橾и 換檜 氈擊 唳辦, п渡 換 嶸雖
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
            // 3偃諦 2偃 嶸雖
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
            // 翱樓脹 4偃 璋濠 嶸雖
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
            // 翱樓脹 5偃 璋濠 嶸雖
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
        printf("橈朝 蘋纔堅葬殮棲棻!\n");
        break;
    }
}

// 闡У攪 薄熱 晦煙
void record_CS(int scores[], int dice[]) {
    int choice_C = choice_BC(scores, dice);
    int score = calc_score(choice_C, dice);
    scores[choice_C] = score; // п渡 蘋纔堅葬縑 薄熱 晦煙
    // printf("Computer scored %d points in category %d.\n", score, choice_C + 1);
    printf("Computer曖 %d廓簞 蘋纔堅葬縑 %d薄熱蒂 橢歷蝗棲棻.\n", choice_C + 1, score);

}

// 啪歜 醞 Ы溯檜橫諦 闡У攪 薄熱 轎溘
void print_MS(int ply[], int com[], int* localData, int* comData) {
    int Ptotal = 0, Ctotal = 0;

    for (int i = 0; i < 6; i++) {
        Ptotal += ply[i];
        Ctotal += com[i];
    }

    printf("%15s忙式式式式式式式式式式成式式式式式式式式式式忖\n", "式成式        ");
    printf("%20s弛%10s弛%10s弛\n", " 弛 弛 弛忙式忙式忖", "", "");
    printf("%19s弛%10s弛%10s弛\n", " 戌 戌式扛戌 戌 戌", "", "");
    printf("%12s弛%10s弛%10s弛\n", "12/12", "player", "computer");
    printf("忙式式式式式式式式式式式扣%10s弛%10s弛\n", "", "");
    printf("弛%11s弛%10s弛%10s弛\n", "Categories", "", "");
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");

    printf("弛%11s弛%10d弛%10d弛\n", "Aces", ply[0], com[0]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Deuces", ply[1], com[1]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Threes", ply[2], com[2]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Fours", ply[3], com[3]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Fives", ply[4], com[4]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Sixes", ply[5], com[5]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%7d%s弛%7d%s弛\n", "Subtotal", Ptotal, "/63", Ctotal, "/63");
    printf("弛***********弛**********弛**********弛\n");

    if (Ptotal == 63) {
        Ptotal += 35;
    }
    if (Ctotal == 63) {
        Ctotal += 35;
    }

    printf("弛%11s弛%10d弛%10d弛\n", "+35 Bonus", Ptotal, Ctotal);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式扛式式式式式式式式式式戎\n");

    printf("Bonus if 1 - 6 are over 63 points\n");

    printf("忙式式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Choice", ply[6], com[6]);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式扛式式式式式式式式式式戎\n");
    printf("忙式式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛%10d弛\n", "4 of a Kind", ply[7], com[7]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Full House", ply[8], com[8]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "S. Straight", ply[9], com[9]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "L. Straight", ply[10], com[10]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Yacht", ply[11], com[11]);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式扛式式式式式式式式式式戎\n");

    for (int i = 6; i < 12; i++) {
        Ptotal += ply[i];
        Ctotal += com[i];
    }

    *localData = Ptotal;
    *comData = Ctotal;

    printf("忙式式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛%10d弛\n", "Total", Ptotal, Ctotal);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式扛式式式式式式式式式式戎\n");

    
    
}


// 寰 噙朝 剪 偽嬴憮 輿戮
// 啪歜 醞 闡У攪 薄熱 轎溘
//void print_CS(int scores[]) {
//    printf("Computer曖 薄熱: ");
//    for (int i = 0; i < NUM_CATEGORIES; i++) {
//        if (scores[i] == -1) printf("[ ] "); // 嘐餌辨 蘋纔堅葬
//        else printf("[%d] ", scores[i]);
//    }
//    printf("\n");
//}

// 啪歜 醞 Ы溯檜橫 薄熱 轎溘
void print_PS(int scores[]) {
    printf("\nPlayer曖 薄熱: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 嘐餌辨 蘋纔堅葬
        else printf("[%d] ", scores[i]); // 晦煙脹 薄熱
    }
    printf("\n");
}

// 臢蝶お 儀鼻 撲薑 л熱
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 輿餌嬪 轎溘 л熱 (醴憮諦 摹鷗 鼻鷓 ル衛)
void display_DWC(int dice[], int keep[], int cursor) {
    system("cls"); // �飛� 蟾晦��
    // printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");
    printf("寞щ酈(<-, ->)蒂 餌辨ж罹 檜翕ж堅, SPACE蒂 摹鷗擊 瞪�納炾�, ENTER蒂 揚楝 �挫恉牉�, ESC蒂 謙猿м棲棻.\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor) {
            setColor(CURSOR_COLOR); // 醴憮縑 嬪纂и 輿餌嬪朝 だ塢儀
        }
        else if (keep[i]) {
            setColor(SELECTED_COLOR); // 摹鷗脹 輿餌嬪朝 喬儀
        }
        else {
            setColor(DEFAULT_COLOR); // 晦獄 儀鼻
        }

        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR); // 儀鼻 蟾晦��
    }
    printf("\n");
}

// 蝶囀橫爾萄 っ
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
    printf("%15s忙式式式式式式式式式式忖\n", "式成式        ");
    printf("%20s弛%10s弛\n", " 弛 弛 弛忙式忙式忖", "");
    printf("%19s弛%10s弛\n", " 戌 戌式扛戌 戌 戌", "");
    printf("%12s弛%10s弛\n", "12/12", "player");
    printf("忙式式式式式式式式式式式扣%10s弛\n", "");
    printf("弛%11s弛%10s弛\n", "Categories", "");
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Aces", ply[0]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Deuces", ply[1]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Threes", ply[2]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Fours", ply[3]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Fives", ply[4]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Sixes", ply[5]);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式戎\n");
    printf("Bonus if 1 - 6 are over 63 points\n");
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%7d%s弛\n", "Subtotal", total, "/63");
    printf("弛***********弛**********弛\n");

    if (total == 63) {
        total += 35;
    }

    printf("弛%11s弛%10d弛\n", "+35 Bonus", total);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式戎\n");

    printf("忙式式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛\n", "Choice", ply[6]);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式戎\n");
    printf("忙式式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛\n", "4 of a Kind", ply[7]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Full House", ply[8]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "S. Straight", ply[9]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "L. Straight", ply[10]);
    printf("戍式式式式式式式式式式式托式式式式式式式式式式扣\n");
    printf("弛%11s弛%10d弛\n", "Yacht", ply[11]);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式戎\n");

    for (int i = 6; i < 12; i++) {
        total += ply[i];
    }

    printf("忙式式式式式式式式式式式成式式式式式式式式式式忖\n");
    printf("弛%11s弛%10d弛\n", "Total", total);
    printf("戌式式式式式式式式式式式扛式式式式式式式式式式戎\n\n");

}

void display_SB(int scores[], int numCategories) {
    scoreBoard(scores);
}

// 譆謙 摹鷗 鼻鷓 �挫� л熱
int select_confirm(int dice[], int keep[]) {
    system("cls");
    // printf("Confirm your selection:\n");
    printf("摹鷗 �挫�:\n");
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i]) {
            setColor(SELECTED_COLOR); // 摹鷗脹 輿餌嬪朝 喬儀
        }
        else {
            setColor(DEFAULT_COLOR); // 晦獄 儀鼻
        }
        printf(" %d ", dice[i]);
    }
    setColor(DEFAULT_COLOR); // 儀鼻 蟾晦��
    // printf("\nPress ENTER to confirm or ESC to go back.\n");
    printf("\nENTER: 啗樓 ESC: 菴煎陛晦\n");
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


// �香嚂� 酈煎 輿餌嬪蒂 摹鷗ж剪釭 嶸雖/п薯 籀葬
void dice_S(int dice[], int keep[]) {
    int cursor = 0; // 醴憮 蟾晦 嬪纂
    int running = 1;

    while (running) {
        display_DWC(dice, keep, cursor); // 輿餌嬪 轎溘

        int key = _getch(); // 酈 殮溘 籀葬
        if (key == ARROW) {
            key = _getch();
            if (key == LEFT) {
                cursor = (cursor == 0) ? NUM_DICE - 1 : cursor - 1; // 豭薹 檜翕
            }
            else if (key == RIGHT) {
                cursor = (cursor == NUM_DICE - 1) ? 0 : cursor + 1; // 螃艇薹 檜翕
            }
        }
        else if (key == SPACEBAR) {
            keep[cursor] = !keep[cursor]; // 摹鷗 鼻鷓 饜旋
        }
        else if (key == ENTER) {
            if (select_confirm(dice, keep)) {
                printf("輿餌嬪蒂 棻衛 掉葬朝 醞殮棲棻...\n");
                Sleep(1000); // 1蟾 渠晦 �� 謙猿
                running = 0;
            }
        }
        else if (key == ESC) {
            printf("Exiting dice selection...\n");
            Sleep(1000); // 1蟾 渠晦 �� 謙猿
            running = 0;
        }
    }
}
