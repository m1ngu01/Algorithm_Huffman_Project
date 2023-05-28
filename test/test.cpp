#define _CRT_SECURE_NO_WARNINGS

//최종수정버전
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define EMPTY_STRING ""
#define MAX_CHAR 128

// 트리 노드
struct Node
{
    char ch;
    int freq;
    struct Node* left, * right;
};

// 빈도수 통계를 저장하기 위한 구조체
typedef struct {
    char character;
    int frequency;
} FrequencyStat;

void countCharacterFrequency(const char* inputFileName, const char* outputFileName) {
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open input file.\n");
        return;
    }


    // 빈도수 통계를 저장할 배열
    FrequencyStat frequencyStats[MAX_CHAR];
    memset(frequencyStats, 0, sizeof(frequencyStats));

    // 파일에서 문자를 하나씩 읽어 빈도수 계산
    int c;
    while ((c = fgetc(inputFile)) != EOF) {
        if (c >= 0 && c < MAX_CHAR) {
            frequencyStats[c].character = c;
            frequencyStats[c].frequency = 0; // 빈도수 초기화
        }
    }

    // 파일에서 문자를 다시 읽어 빈도수 증가
    rewind(inputFile);
    while ((c = fgetc(inputFile)) != EOF) {
        if (c >= 0 && c < MAX_CHAR) {
            frequencyStats[c].frequency++;
        }
    }

    fclose(inputFile);

    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Failed to open output file.\n");
        return;
    }

    // 빈도수 통계를 파일에 저장
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequencyStats[i].frequency > 0) {
            fprintf(outputFile, "%c\t%d\n", frequencyStats[i].character, frequencyStats[i].frequency);
        }
    }

    fclose(outputFile);

    printf("Character frequency statistics saved to %s.\n", outputFileName);
}

// 새로운 트리 노드를 할당하는 함수
struct Node* getNode(char ch, int freq, struct Node* left, struct Node* right)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Huffman 트리를 탐색하고 지도에 Huffman 코드를 저장합니다.
void encode(struct Node* root, const char* str, char* huffmanCode[])
{
    if (root == NULL)
        return;

    // 리프 노드를 찾았습니다.
    if (root->left == NULL && root->right == NULL)
    {
        int index = root->ch - 'A';
        strcpy(huffmanCode[index], str != EMPTY_STRING ? str : "1");
    }

    int len = strlen(str);
    char* leftStr = (char*)malloc(sizeof(char) * (len + 2));
    char* rightStr = (char*)malloc(sizeof(char) * (len + 2));

    strcpy(leftStr, str);
    strcpy(rightStr, str);

    leftStr[len] = '0';
    leftStr[len + 1] = '\0';

    rightStr[len] = '1';
    rightStr[len + 1] = '\0';

    encode(root->left, leftStr, huffmanCode);
    encode(root->right, rightStr, huffmanCode);

    free(leftStr);
    free(rightStr);
}

//decod 함수 추가
void decode(struct Node* root, int* index, char* encodedStr)
{
    if (root == NULL)
        return;

    // 리프 노드를 찾았습니다.
    if (root->left == NULL && root->right == NULL)
    {
        printf("%c", root->ch);
        return;
    }

    (*index)++;

    if (encodedStr[*index] == '0')
        decode(root->left, index, encodedStr);
    else
        decode(root->right, index, encodedStr);
}


// Huffman Tree를 구축하고 주어진 입력 텍스트를 디코딩합니다.
void buildHuffmanTree(char* text)
{
    if (strlen(text) == 0)
        return;

    // 각 문자의 출현 빈도를 계산합니다.
    int freq[26] = { 0 };
    int i;
    for (i = 0; i < strlen(text); i++)
    {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            freq[ch - 'A']++;
        else if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }

    // Huffman Tree의 리프 노드를 저장할 배열을 생성합니다.
    struct Node* leaves[26];
    for (i = 0; i < 26; i++)
        leaves[i] = NULL;

    // 각 문자에 대한 리프 노드를 생성하고 배열에 저장합니다.
    for (i = 0; i < 26; i++)
    {
        if (freq[i] > 0)
            leaves[i] = getNode('A' + i, freq[i], NULL, NULL);
    }

    // Huffman Tree의 내부 노드를 저장할 큐를 생성합니다.
    struct Node** queue = (struct Node**)malloc(sizeof(struct Node*) * MAX_TREE_HT);
    int queueSize = 0;

    // 리프 노드를 큐에 추가합니다.
    for (i = 0; i < 26; i++)
    {
        if (leaves[i] != NULL)
            queue[queueSize++] = leaves[i];
    }

    // Huffman Tree를 구축합니다.
    struct Node* root = NULL;
    while (queueSize > 1)
    {
        // 가장 작은 빈도를 가진 두 개의 노드를 선택합니다.
        struct Node* min1 = queue[0];
        struct Node* min2 = queue[1];

        // 선택된 두 노드를 큐에서 제거합니다.
        for (i = 0; i < queueSize - 2; i++)
            queue[i] = queue[i + 2];
        queueSize -= 2;

        // 새로운 내부 노드를 생성하고 선택된 두 노드를 자식으로 설정합니다.
        struct Node* internalNode = getNode('\0', min1->freq + min2->freq, min1, min2);

        // 새로운 내부 노드를 큐에 추가합니다.
        int j = queueSize - 1;
        while (j >= 0 && internalNode->freq < queue[j]->freq)
        {
            queue[j + 1] = queue[j];
            j--;
        }
        queue[j + 1] = internalNode;
        queueSize++;
    }

    // Huffman 코드를 저장할 배열을 생성합니다.
    char* huffmanCode[26];
    for (i = 0; i < 26; i++)
        huffmanCode[i] = (char*)malloc(sizeof(char) * MAX_TREE_HT);

    // Huffman Tree를 탐색하고 Huffman 코드를 저장합니다.
    encode(queue[0], EMPTY_STRING, huffmanCode);

    printf("Huffman Codes are:\n");
    for (i = 0; i < 26; i++)
    {
        if (leaves[i] != NULL)
            printf("%c %s\n", leaves[i]->ch, huffmanCode[i]);
    }

    printf("\nThe original string is:\n%s\n", text);

    // 인코딩된 문자열을 생성합니다.
    int len = strlen(text);
    char* encodedStr = (char*)malloc(sizeof(char) * (len * MAX_TREE_HT));
    encodedStr[0] = '\0';

    for (i = 0; i < len; i++)
    {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            strcat(encodedStr, huffmanCode[ch - 'A']);
        else if (ch >= 'a' && ch <= 'z')
            strcat(encodedStr, huffmanCode[ch - 'a']);
    }

    printf("\nThe encoded string is:\n%s\n", encodedStr);

    printf("\nThe decoded string is:\n");
    if (queue[0] == NULL)
    {
        printf("Huffman Tree is empty.\n");
        return;
    }

    int index = -1;
    while (index < len - 1)
        decode(queue[0], &index, encodedStr);

    printf("\n");

    // 메모리 해제
    for (i = 0; i < 26; i++)
        free(huffmanCode[i]);

    FILE* outputFile = fopen("output.huf", "w");
    if (outputFile == NULL) {
        printf("Failed to open output file.\n");
        return;
    }

    // 압축된 데이터를 파일에 저장
    fputs(encodedStr, outputFile);

    fclose(outputFile);


    free(encodedStr);
    free(queue);
}

//압축해제
void decodebuildHuffmanTree(const char* statsFileName, const char* compressedFileName, const char* outputFileName) {
    // stats.txt 파일을 읽어와서 Huffman Tree를 재구성합니다.

    // 압축된 데이터를 읽어올 파일을 엽니다.
    FILE* compressedFile = fopen(compressedFileName, "r");
    if (compressedFile == NULL) {
        printf("Failed to open compressed file.\n");
        return;
    }

    // output.txt 파일을 생성하고 엽니다.
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Failed to open output file.\n");
        fclose(compressedFile); // 파일을 닫습니다.
        return;
    }

    // 압축 해제를 위해 Huffman Tree를 사용합니다.
    // compressedFile에서 압축된 데이터를 읽어와서 Huffman Tree를 따라가며 해제하고,
    // 해제된 데이터를 output.txt 파일에 저장합니다.
    // 이 부분은 Huffman Tree를 따라가며 압축을 해제하는 코드를 구현해야 합니다.

    fclose(compressedFile); // 파일을 닫습니다.
    fclose(outputFile); // 파일을 닫습니다.
}




int main()
{
    const char* inputFileName = "input.txt";
    const char* outputFileName = "output.huf";
    const char* statsFileName = "stats.txt";

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }

    // 입력 파일의 내용을 읽어옵니다.
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);

    char* fileContent = (char*)malloc(sizeof(char) * (fileSize + 1));
    fread(fileContent, sizeof(char), fileSize, inputFile);
    fileContent[fileSize] = '\0';

    fclose(inputFile);

    // Huffman Tree를 구축하고 압축을 수행합니다.
    buildHuffmanTree(fileContent);

    // 압축된 결과를 파일에 저장합니다.
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Failed to open output file.\n");
        return 1;
    }


    fwrite(fileContent, sizeof(char), strlen(fileContent), outputFile);

    fclose(outputFile);

    printf("File compressed and saved to %s.\n", outputFileName);

    countCharacterFrequency(inputFileName, statsFileName);

    free(fileContent);

    decodebuildHuffmanTree("stats.txt", "output.huf", "output.txt");


    return 0;
}