#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE    512

// �۽� ���� ������
DWORD WINAPI SendThread(LPVOID lpParam) {
    SOCKET sock = (SOCKET)lpParam;
    char buf[BUF_SIZE];

    while (fgets(buf, BUF_SIZE, stdin)) {
        int len = (int)strlen(buf);
        if (send(sock, buf, len, 0) == SOCKET_ERROR) {
            printf("send error: %d\n", WSAGetLastError());
            break;
        }
    }
    return 0;
}

// ���� ���� ������
DWORD WINAPI RecvThread(LPVOID lpParam) {
    SOCKET sock = (SOCKET)lpParam;
    char buf[BUF_SIZE];
    int ret;

    while ((ret = recv(sock, buf, BUF_SIZE, 0)) > 0) {
        printf("Remote �� %.*s", ret, buf);
    }
    if (ret == 0) {
        printf("Connection closed by peer.\n");
    }
    else {
        printf("recv error: %d\n", WSAGetLastError());
    }
    return 0;
}

void server_mode(const char* port) {
    WSADATA wsa;
    SOCKET listenSock = INVALID_SOCKET, clientSock = INVALID_SOCKET;
    struct addrinfo hints = { 0 }, * res = NULL;

    if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return;
    }

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        printf("getaddrinfo failed\n");
        goto cleanup;
    }

    listenSock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    if (bind(listenSock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    if (listen(listenSock, 1) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    printf("== �� ���� �Ϸ�: ��Ʈ %s, Ŭ���̾�Ʈ ���� ��� �� ==\n", port);
    clientSock = accept(listenSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        goto cleanup;
    }
    printf("Ŭ���̾�Ʈ ���ӵ�! ������ �ۼ��� ����...\n");

    // ��/���� ������ ����
    HANDLE hSend = CreateThread(NULL, 0, SendThread, (LPVOID)clientSock, 0, NULL);
    HANDLE hRecv = CreateThread(NULL, 0, RecvThread, (LPVOID)clientSock, 0, NULL);
    WaitForSingleObject(hRecv, INFINITE);  // ���� ������ ���� ���
    // (�۽� ������� stdin EOF �� ����)
    WaitForSingleObject(hSend, INFINITE);

cleanup:
    if (listenSock != INVALID_SOCKET) closesocket(listenSock);
    if (clientSock != INVALID_SOCKET) closesocket(clientSock);
    WSACleanup();
}

void client_mode(const char* ip, const char* port) {
    WSADATA wsa;
    SOCKET sock = INVALID_SOCKET;
    struct addrinfo hints = { 0 }, * res = NULL;

    if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return;
    }

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(ip, port, &hints, &res) != 0) {
        printf("getaddrinfo failed\n");
        goto cleanup;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        printf("connect failed: %d\n", WSAGetLastError());
        goto cleanup;
    }
    printf("== ������ ���� ����! ������ �ۼ��� ����... ==\n");

    HANDLE hSend = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, NULL);
    HANDLE hRecv = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);
    WaitForSingleObject(hRecv, INFINITE);
    WaitForSingleObject(hSend, INFINITE);

cleanup:
    if (sock != INVALID_SOCKET) closesocket(sock);
    WSACleanup();
}

int main() {
    int choice;
    char ip[128], port[6];

    printf("=== Winsock2 Room ���� ===\n");
    printf("1) �� ���� (ȣ��Ʈ)\n");
    printf("2) �� ���� (Ŭ���̾�Ʈ)\n");
    printf("����: ");
    if (scanf("%d%*c", &choice) != 1) {
        printf("�߸��� �Է�\n");
        return 1;
    }

    switch (choice) {
    case 1:
        printf("����� ��Ʈ ��ȣ �Է�: ");
        fgets(port, sizeof(port), stdin);
        port[strcspn(port, "\r\n")] = 0;
        server_mode(port);
        break;
    case 2:
        printf("������ ���� IP �Է�: ");
        fgets(ip, sizeof(ip), stdin);
        ip[strcspn(ip, "\r\n")] = 0;
        printf("��Ʈ ��ȣ �Է�: ");
        fgets(port, sizeof(port), stdin);
        port[strcspn(port, "\r\n")] = 0;
        client_mode(ip, port);
        break;
    default:
        printf("1 �Ǵ� 2�� �Է��ϼ���.\n");
    }

    return 0;
}
