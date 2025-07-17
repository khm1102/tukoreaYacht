#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>

// 주사위 숫자 패턴 (6면 × 5행)
extern const char* dicePatterns[6][5];

// 주사위를 가로로 출력
void displayAllDiceHorizontal(int dice[], int numDice);

// 점수판 출력
void displayScoreboard(int scores[], int numCategories);

// 시작 화면 출력
void displayWelcomeScreen(void);

#endif // GRAPHICS_H
