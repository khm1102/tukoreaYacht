#include "localUser.h"


typedef struct {
    int local;   // ����� ����
    int com;     // ���(��ǻ��) ����
} ScoreData;

/*
 * ���� ���� �״���� ������ ��ȯ
 */
void printAllUserData(void) {
    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("���� ���� ����");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, stdout);
    }
    fclose(fp);
}

/*
 * ����� �� ������ �ٸ� �÷��̾� ������ ���Ͽ� �� �� �߰� ����
 */
void saveLocalData(int localTotal, int CTotal) {
    FILE* fp = fopen("localUser.txt", "a");
    if (!fp) {
        perror("���� ���� ����");
        return;
    }
    fprintf(fp, "%d,%d\n", localTotal, CTotal);
    fclose(fp);
}

/*
 * �÷����� Ƚ��(������ �� ����) ��ȯ
 */
int localPlayCnt(void) {
    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("���� ���� ����");
        return 0;
    }
    char line[256];
    int count = 0;
    // ù ��(���) �ǳʶٱ�
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    // ������ �� ����
    while (fgets(line, sizeof(line), fp)) {
        // �� ���� �ƴϸ� ī��Ʈ
        if (strspn(line, "\r\n") != strlen(line))
            count++;
    }
    fclose(fp);
    return count;
}

// qsort �񱳿� ����� ���� ���� ����
static int sortOrder = 1;
static int compareScore(const void* a, const void* b) {
    const ScoreData* s1 = a;
    const ScoreData* s2 = b;
    return sortOrder * (s1->local - s2->local);
}

/*
 * �ڽ��� ���� �������� ����/�������� ���� �� ȭ�鿡 ���
 *  x == 1  : ��������
 *  x == -1 : ��������
 */
void localPlaySort(int x) {
    sortOrder = (x >= 0 ? 1 : -1);

    FILE* fp = fopen("localUser.txt", "r");
    if (!fp) {
        perror("���� ���� ����");
        return;
    }

    char line[256];
    // ��� �ǳʶٱ�
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    // ������ �о���̱�
    ScoreData* arr = NULL;
    size_t cap = 0, size = 0;
    while (fgets(line, sizeof(line), fp)) {
        int l, c;
        if (sscanf(line, "%d,%d", &l, &c) == 2) {
            if (size == cap) {
                cap = cap ? cap * 2 : 8;
                arr = realloc(arr, cap * sizeof(ScoreData));
                if (!arr) {
                    perror("�޸� �Ҵ� ����");
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

    // ���
    printf("=== %s ���� ��� ===\n", (sortOrder == 1 ? "��������" : "��������"));
    printf("localData, comData\n");
    for (size_t i = 0; i < size; i++) {
//        printf("%d,%d\n", arr[i].local, arr[i].com);
        printf("%d,%d\n", arr[i].local);

    }

    free(arr);
}
