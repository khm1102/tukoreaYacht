#pragma once
#ifndef __UTILS__
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include "graphics.h"


#define NUM_DICE 5
#define NUM_CATEGORIES 12
//Ű���� �Է°�
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define ENTER 13
#define ESC 27
//��µǴ� ���� ��
#define SELECTED_COLOR 10 // ���
#define CURSOR_COLOR 9    // �Ķ���
#define DEFAULT_COLOR 15  // �⺻ ����


void rollDice(int dice[], int keep[]);
void displayDice(int dice[]);

int calc_score(int category, int dice[]);
int check_Y(int dice[]);
int check_FOK(int dice[]);
int check_FH(int dice[]);
int check_LS(int dice[]);
int check_BS(int dice[]);
int calc_sum(int dice[], int num);
int calc_upper_s(int scores[]);

int choice_BC(int scores[], int dice[]);
void decide_KD(int dice[], int keep[], int targetCategory);
void record_CS(int scores[], int dice[]);

void print_PS(int scores[]);
void print_CS(int scores[]);
void setColor(int color);
void display_SB(int scores[], int numCategories);
int select_confirm(int dice[], int keep[]);
void dice_S(int dice[], int keep[]);


#endif