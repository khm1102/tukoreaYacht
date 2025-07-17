#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>

// �ֻ��� ���� ���� (6�� �� 5��)
extern const char* dicePatterns[6][5];

// �ֻ����� ���η� ���
void displayAllDiceHorizontal(int dice[], int numDice);

// ������ ���
void displayScoreboard(int scores[], int numCategories);

// ���� ȭ�� ���
void displayWelcomeScreen(void);

#endif // GRAPHICS_H
