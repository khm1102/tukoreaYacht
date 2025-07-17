#pragma once

#ifndef LOCALUSER_H
#define LOCALUSER_H
#include "utils.h"

// 게임 종료 이후 local 총 점수와 cTotal 게임 데이터를 파일에 저장
void saveLocalData(int localTotal, int CTotal);

// 게임 내역에 대한 오름차순, 내림차순으로 사용자 콘솔에 출력
void localDataSort(int x);

void printAllUserData();



#endif