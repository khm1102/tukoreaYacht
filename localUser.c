#include "localUser.h"

typedef struct {
    int local;   // 사용자 점수
    int com;     // 상대(컴퓨터) 점수
} ScoreData;

/*
 * 원본 파일 그대로의 데이터 반환 // select all
 */
void printAllUserData(void) {
    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("파일 열기 실패");
        return;
    }

    // 메모리 동적 할당을 위한 준비
    ScoreData* arr = NULL;
    size_t cap = 0, size = 0;
    char line[256];

    // 파일의 각 줄을 읽어서 배열에 저장
    while (fgets(line, sizeof(line), fp)) {
        int l, c;
        if (sscanf(line, "%d,%d", &l, &c) == 2) {
            if (size == cap) {
                cap = cap ? cap * 2 : 8;
                arr = realloc(arr, cap * sizeof(ScoreData));
                if (!arr) {
                    perror("메모리 할당 실패");
                    fclose(fp);
                    return;
                }
            }
            arr[size].local = l;
            arr[size].com = c;
            size++;
        }
    }
    fclose(fp);

    // 화면 지우기 (원하지 않으면 주석 처리)
    system("cls");

    // 표 헤더
    printf("=== 전체 플레이 기록 (총 %zu회) ===\n\n", size);
    printf("+------+------------+------------+\n");
    printf("|  순번 |  로컬 점수  | 컴퓨터 점수 |\n");
    printf("+------+------------+------------+\n");

    // 데이터 출력
    for (size_t i = 0; i < size; i++) {
        printf("| %4zu | %10d | %10d |\n",
            i + 1, arr[i].local, arr[i].com);
    }
    printf("+------+------------+------------+\n");

    free(arr);
}


/*
 * 사용자 총 점수와 다른 플레이어 점수를 파일에 한 줄 추가 저장 append data
 */
void saveLocalData(int localTotal, int CTotal) {
    FILE* fp = fopen("localUser.txt", "a");
    if (!fp) {
        perror("파일 열기 실패");
        return;
    }
    fprintf(fp, "%d,%d\n", localTotal, CTotal);
    fclose(fp);
}

/*
 * 플레이한 횟수(데이터 행 개수) 반환
 */
int localPlayCnt(void) {
    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("파일 열기 실패");
        return 0;
    }
    char line[256];
    int count = 0;
    // 첫 줄(헤더) 건너뛰기
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    // 나머지 줄 세기
    while (fgets(line, sizeof(line), fp)) {
        // 빈 줄이 아니면 카운트
        if (strspn(line, "\r\n") != strlen(line))
            count++;
    }
    fclose(fp);
    return count;
}

// qsort 비교에 사용할 전역 정렬 방향
static int sortOrder = 1;
static int compareScore(const void* a, const void* b) {
    const ScoreData* s1 = a;
    const ScoreData* s2 = b;
    return sortOrder * (s1->local - s2->local);
}

/*
 * 자신의 점수 기준으로 오름/내림차순 정렬 후 화면에 출력
 *  x == 1  : 오름차순
 *  x == -1 : 내림차순
 */
void localPlaySort(int x) {
    sortOrder = (x >= 0 ? 1 : -1);

    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("파일 열기 실패");
        return;
    }

    char line[256];
    // 헤더(첫 줄) 건너뛰기
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    ScoreData* arr = NULL;
    size_t cap = 0, size = 0;
    while (fgets(line, sizeof(line), fp)) {
        int l, c;
        if (sscanf(line, "%d,%d", &l, &c) == 2) {
            if (size == cap) {
                cap = cap ? cap * 2 : 8;
                arr = realloc(arr, cap * sizeof(ScoreData));
                if (!arr) {
                    perror("메모리 할당 실패");
                    fclose(fp);
                    return;
                }
            }
            arr[size].local = l;
            arr[size].com = c;
            size++;
        }
    }
    fclose(fp);

    qsort(arr, size, sizeof(ScoreData), compareScore);

    // 화면 클리어 후 예쁜 표로 출력
    system("cls");
    printf("=== %s 정렬 결과 (총 %zu회 플레이) ===\n\n",
        (sortOrder == 1 ? "오름차순" : "내림차순"), size);
    printf("+------+------------+------------+\n");
    printf("|  순위 |  로컬 점수  | 컴퓨터 점수 |\n");
    printf("+------+------------+------------+\n");
    for (size_t i = 0; i < size; i++) {
        printf("| %4zu | %10d | %10d |\n",
            i + 1, arr[i].local, arr[i].com);
    }
    printf("+------+------------+------------+\n");

    free(arr);
}
