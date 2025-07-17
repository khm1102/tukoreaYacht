// game.c
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "utils.h"
#include "game.h"

void playMode1() {
    int dice[NUM_DICE];
    int PS[NUM_CATEGORIES];
    int keep[NUM_DICE];

    // 점수 초기화
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        PS[i] = -1;
    }
    // 보관 플래그 초기화
    for (int i = 0; i < NUM_DICE; i++) {
        keep[i] = 0;
    }

    srand((unsigned)time(NULL));

    printf("\n=== Solo Mode ===\n");
    // 12가지 카테고리 턴 반복
    for (int turn = 0; turn < NUM_CATEGORIES; turn++) {
        printf("\n--- Turn %d / %d ---\n", turn + 1, NUM_CATEGORIES);

        // 매 턴마다 keep 리셋
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 0;
        }

        // 최대 3번 굴리기
        for (int roll = 0; roll < 3; roll++) {
            rollDice(dice, keep);
            printf("\n계속하려면 아무 키나 누르세요...\n");
            _getch();
            printf("\n%d번째 굴리기 결과:\n", roll + 1);
            displayDice(dice);

            if (roll < 2) {
                printf("보관하거나 다시 굴릴 주사위를 선택하세요.\n");
                dice_S(dice, keep);
            }
        }

        // 카테고리 선택 및 점수 확정
        int chosen = 0;
        while (!chosen) {
            displayDice(dice);
            print_PS(PS);
            printf("카테고리를 선택하세요 (1:Ones … 12:Choice): ");
            int category;
            scanf("%d", &category);
            category--;

            if (category >= 0 && category < NUM_CATEGORIES && PS[category] == -1) {
                PS[category] = calc_score(category, dice);
                printf("획득 점수: %d\n", PS[category]);
                chosen = 1;
            }
            else {
                printf("유효하지 않거나 이미 선택된 카테고리입니다. 다시 선택하세요.\n");
            }
        }
    }

    // 최종 점수 집계 및 출력
    printf("\n=== 최종 점수 ===\n");
    print_PS(PS);
    
    int total = 0;
    int upper = calc_upper_s(PS);
    if (upper >= 63) {
        printf("상위합 63점 달성! 보너스 35점 지급!\n");
        total += 35;
    }
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (PS[i] != -1) total += PS[i];
    }
    printf("총점: %d\n", total);

    printf("\n결과 보드:\n");
    display_SB(PS, NUM_CATEGORIES);
    Sleep(10);
}
