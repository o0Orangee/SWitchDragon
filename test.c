#pragma warning(disable:4996)
#pragma comment (lib, "winmm.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#include <Digitalv.h>

#include "block.h"

// user controls
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define SPACE 32
#define R 114 // ultimate
#define ESC 27
#define GB 120 // x for go back

// size of gameboard
#define GBOARD_WIDTH 26
#define GBOARD_HEIGHT 18

// starting point of gameboard
#define GBOARD_ORIGIN_X 14
#define GBOARD_ORIGIN_Y 5

// size of storyboard
#define SBOARD_WIDTH 27
#define SBOARD_HEIGHT 18

// starting point of storyboard
#define SBOARD_ORIGIN_X 35
#define SBOARD_ORIGIN_Y 6

// size of ultimate board
#define UBOARD_WIDTH 7
#define UBOARD_HEIGHT 2

// starting point of ultimate board
#define UBOARD_ORIGIN_X 13
#define UBOARD_ORIGIN_Y 24

// size of fever board
#define FBOARD_WIDTH 7
#define FBOARD_HEIGHT 2

// starting point of fever board
#define FBOARD_ORIGIN_X 53
#define FBOARD_ORIGIN_Y 24

// size of controls board
#define CBOARD_WIDTH 8
#define CBOARD_HEIGHT 9

// starting point of controls board
#define CBOARD_ORIGIN_X 71
#define CBOARD_ORIGIN_Y 13

// size of story controls board
#define SCBOARD_WIDTH 8
#define SCBOARD_HEIGHT 9

// starting point of story controls board
#define SCBOARD_ORIGIN_X 37
#define SCBOARD_ORIGIN_Y 8

#define INFO_X 77

#define ITEM_Y 23
#define PAUSE_Y 17

#define TARGET_SCORE 50

typedef struct Moogi {
    struct Moogi* front;
    struct Moogi* back;
    COORD position;
    char direct;
} Moogi;

typedef struct _Item {
    COORD pos;
    int itemNo;
} Item, * pItem;

typedef struct Human {
    int direct;
    COORD position;
    struct Human* next;
} Human;

typedef struct _Pet {
    COORD pos;
} Pet;

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int direction = 3;
int stage = 0;
int stageUnlock[5] = { 0, 0, 0, 0, 0 };
int stageBestScore[5] = { 0, 0, 0, 0, 0 };
int speed = 15;
int heart = 3;
int dragonBallCount = 0;
int itemCount = 0;

//FEVER related variables
int fever[5] = { 0,0,0,0,0 };
int feverStart = 0;
int feverScore = 0;
time_t feverWordCreateTime;
time_t feverStartTime;
COORD feverWordPos = { 0,0 };

Moogi* head = NULL;
Moogi* body = NULL;
Moogi* tail = NULL;

pItem item = NULL;
time_t itemCreationTime;

int length = 3;
int minLength = 2;
int maxLength = 40;
int minSpeed = 5;
int maxSpeed = 25;
int speedScore = 50;

int currentScore = 0;

Human* humanHead;
int humanNum = 0;

int moogiColor = 4; // 5���� ���� ���. 0��4(dark red), 1��6(dark yellow), 2��2(dark green), 3��9(blue), 4��15(white)
int dragonBallColor[5]; // 0�� 1�� 2�� 3�� 4�� ����, ������ ��ġ���������Ŵϱ� �� ������ �������
COORD dragonBallPos[5]; // ���������� ������� ��ġ ����. �˸��� ������ �����ָ� ������ ���������� ��������ϱ⶧��..��..

int petGauge = 5;
int petScore = 0;
Pet* pet = NULL;
time_t petCreationTime;

COORD wormholePos[2];

int stage5InPlay = 0;
int turnOnAndOff = 1;
int stage5map = 0;
COORD stage5DragonBallPos[5];
int stage5DragonBallNum = 0;
time_t stage5ShowDragonBall[5];

int humanAttack = 0;

void reset();
void resetGameBoardInfo();

// stages
void stage1();
void stage2();
void stage3();
void stage4();
void stage5();

// Map related functions;
void gotoxycol(int x, int y, int col, char* s);
void introScreen();
void startScreen();
void story();
void tutorial();
void selectStage();
void drawBoard();
void drawUlt();
void drawFever();
void printStage();
void printScore();
void printHighscore();
void drawLife();
void drawControls();
void drawKeys();
void clearScreen();
void clearControls();
void stageClr();
void ending();
void gameOver();

// console related functions
void console();
void setCurrentCursorPos(int x, int y);
COORD getCurrentCursorPos();
void removeCursor();
void gotoxy(int x, int y, char* ch);
void setTextColor(int colorNum);

void initGameBoardInfo();
void drawBoard();

void initMoogi();
void addBody();
void deleteBody();
void speedUp();
void speedDown();
void createItem();
void deleteItem();
void getHeart();
void dragonBallBomb();
void getPet();
void usePet();
void petMove();
void getItemOfPet();
void moogiSwitch();
void waitToRecover();

void inPlayKeyInput();
void shiftUp();
void shiftDown();
void shiftLeft();
void shiftRight();
void pausePlay();
int detectCollision(int currentPosX, int currentPosY);
COORD nextHeadPos();
void drawHead(COORD headPos);
void eraseTail();
Moogi* getNode(Moogi* front, Moogi* back, COORD position);
int moogiMove();
void getSomething();
void humanMove();

void countScore();
void getBestScore();
void setBestScore(int score);
void saveBestScore();
int isGameOver();
void getItem();
void addDragonBall();
void showDragonBall(int arrX, int arrY);
void addHuman();
void addBlock();

void setTextColor_rygbw(int colorNum);
void changeMoogiColor();
void changeMoogiBodyColor();
void setDragonBallColor();
void setDragonBallPos();
void showColorDragonBall(int x, int y, int color);
void deleteDragonBall();

void createFever();
void deleteFeverWord();
void getFever();
int isFever();
void fillFever();
void removeFever();

void wormhole();

void isHumanAttack();

int isIn(int x, int y);
int nearTheHead(int x, int y);
void showTurnOnArea();
void addGameBoardInfo();
void showDragonBallPos();

// ��� ����
MCI_OPEN_PARMS openOpeningBgm;      // dwID1
MCI_PLAY_PARMS playOpeningBgm;

MCI_OPEN_PARMS openPlayingBgm;      // dwID2
MCI_PLAY_PARMS playPlayingBgm;

MCI_OPEN_PARMS openSelectSound;     // dwID3
MCI_PLAY_PARMS playSelectSound;

MCI_OPEN_PARMS openGetSound;        // dwID4
MCI_PLAY_PARMS playGetSound;

MCI_OPEN_PARMS openCollisionSound;  // dwID5
MCI_PLAY_PARMS playCollisionSound;

MCI_OPEN_PARMS openClearSound;      // dwID6
MCI_PLAY_PARMS playClearSound;

MCI_OPEN_PARMS openGameoverSound;   // dwID7
MCI_PLAY_PARMS playGameoverSound;

MCI_OPEN_PARMS openTypeSound;   // dwID8
MCI_PLAY_PARMS playTypeSound;

MCI_OPEN_PARMS openThunderSound;   // dwID9
MCI_PLAY_PARMS playThunderSound;

int dwID1, dwID2, dwID3, dwID4, dwID5, dwID6, dwID7, dwID8, dwID9;

void openingBGM(int play);
void playingBGM(int play);
void selectSound();
void getSound();
void collisionSound();
void clearSound();
void gameoverSound();
void typeSound();
void thunderSound();

void printMatrix();

int main() {
    console();
    removeCursor();
    introScreen();
    startScreen();
    getBestScore();

    while (1)
    {
        if (stage != 0 && stage != 5 && stageUnlock[stage] == 1)
        {
            stageUnlock[stage] = 2;
            stage++;
        }
        else
        {
            system("cls");
            selectStage();
        }

        system("cls");
        switch (stage)
        {
        case 1:
            clearScreen();
            reset();
            stage1();
            break;
        case 2:
            clearScreen();
            reset();
            stage2();
            break;
        case 3:
            clearScreen();
            reset();
            stage3();
            break;
        case 4:
            clearScreen();
            reset();
            stage4();
            break;
        case 5:
            clearScreen();
            reset();
            stage5();
            break;
        default:
            break;
        }
    }
    getch();

    return 0;
}

void stage1() {
    drawBoard();

    srand((unsigned int)time(NULL));
    initMoogi();
    addDragonBall();
    createItem();

    int key = 0;

    playingBGM(0);
    while (1) {
        if (isGameOver()) break;

        if (key == ENTER) break;

        while (1) {
            if (moogiMove() == 0) break;

            if (stageBestScore[0] < TARGET_SCORE && currentScore >= TARGET_SCORE)
            {
                stageUnlock[1] = 1;
                playingBGM(1);
                clearScreen();
                stageClr();
                while (1) {
                    if (_kbhit()) {
                        key = _getch();
                        if (key == ENTER) break;
                    }
                }
            }
            if (key == ENTER) break;

            inPlayKeyInput();
        }
    }

    setBestScore(currentScore);
    saveBestScore();

    if (key == ENTER) return;

    playingBGM(1);
    gameOver();
}

void stage2() {
    drawBoard();

    srand((unsigned int)time(NULL));
    initMoogi();
    addDragonBall();
    createItem();

    humanHead = (Human*)malloc(sizeof(Human));
    humanHead->next = NULL;

    int key = 0;

    playingBGM(0);
    while (1) {
        if (isGameOver()) break;

        if (key == ENTER) break;

        while (1) {
            if (moogiMove() == 0) break;
            //if (humanAttack) break;

            if (stageBestScore[1] < TARGET_SCORE && currentScore >= TARGET_SCORE)
            {
                stageUnlock[2] = 1;
                playingBGM(1);
                clearScreen();
                stageClr();
                while (1) {
                    if (_kbhit()) {
                        key = _getch();
                        if (key == ENTER) break;
                    }
                }
            }
            if (key == ENTER) break;

            inPlayKeyInput();

        }
    }
    setBestScore(currentScore);
    saveBestScore();
    setTextColor(4);

    if (key == ENTER) return;

    playingBGM(1);
    gameOver();
}

void stage3() {
    drawBoard();

    srand((unsigned int)time(NULL));
    setDragonBallColor();
    setDragonBallPos();
    initMoogi();
    createItem();
    addDragonBall();

    int key = 0;

    playingBGM(0);
    while (1) {
        if (isGameOver()) break;

        if (key == ENTER) break;

        while (1) {
            if (moogiMove() == 0) break;

            if (stageBestScore[2] < TARGET_SCORE && currentScore >= TARGET_SCORE)
            {
                stageUnlock[3] = 1;
                playingBGM(1);
                clearScreen();
                stageClr();
                while (1) {
                    if (_kbhit()) {
                        key = _getch();
                        if (key == ENTER) break;
                    }
                }
            }
            if (key == ENTER) break;

            //printMatrix();

            inPlayKeyInput();
        }
    }
    setBestScore(currentScore);
    saveBestScore();

    if (key == ENTER) return;

    playingBGM(1);
    gameOver();
}

void stage4() {
    drawBoard();

    srand((unsigned int)time(NULL));
    initMoogi();
    addDragonBall();
    createItem();

    int key = 0;

    playingBGM(0);
    while (1) {
        if (isGameOver()) break;

        if (key == ENTER) break;

        while (1) {
            if (moogiMove() == 0) break;

            if (stageBestScore[3] < TARGET_SCORE && currentScore >= TARGET_SCORE)
            {
                stageUnlock[4] = 1;
                playingBGM(1);
                clearScreen();
                stageClr();
                while (1) {
                    if (_kbhit()) {
                        key = _getch();
                        if (key == ENTER) break;
                    }
                }
            }
            if (key == ENTER) break;

            inPlayKeyInput();
        }
    }
    setBestScore(currentScore);
    saveBestScore();

    if (key == ENTER) return;

    playingBGM(1);
    gameOver();
}

void stage5()
{
    drawBoard();

    srand((unsigned int)time(NULL));

    initMoogi();

    stage5InPlay = 1;
    addGameBoardInfo();
    addDragonBall();
    createItem();

    int key = 0;
    stage5map = rand() % 4;

    playingBGM(0);
    while (1)
    {
        if (isGameOver()) break;

        if (key == ENTER) break;

        while (1)
        {
            showTurnOnArea();
            showDragonBallPos();
            if (moogiMove() == 0) break;

            if (stageBestScore[4] < TARGET_SCORE && currentScore >= TARGET_SCORE)
            {
                stage5InPlay = 0;
                //stageUnlock[4] = 1;
                playingBGM(1);
                clearScreen();
                stageClr();
                while (1)
                {
                    if (_kbhit())
                    {
                        key = _getch();
                        if (key == ENTER) {
                            system("cls");
                            ending();
                            break;
                        }
                    }
                }
            }
            if (key == ENTER) break;

            inPlayKeyInput();
            //turnOn();
        }
    }
    stage5InPlay = 0;

    setBestScore(currentScore);
    saveBestScore();

    if (key == ENTER) return;

    playingBGM(1);
    gameOver();
}

void printMatrix()
{
    for (int i = 0; i < GBOARD_HEIGHT + 1; i++)
    {
        setCurrentCursorPos(INFO_X, PAUSE_Y + 2 + i);
        for (int j = 0; j < GBOARD_WIDTH + 2; j++)
        {
            if (!gameBoardInfo[i][j]) printf("  ");
            else if (tail->position.X == j * 2 + GBOARD_ORIGIN_X && tail->position.Y == i + GBOARD_ORIGIN_Y)
                printf("6 ");
            else printf("%d ", gameBoardInfo[i][j]);
        }
    }
}

/*-----------------------------------------------------------------------------------------*/

void console() { // set console size
    system("mode con:cols=125 lines=33"); // 125 33
}

void setCurrentCursorPos(int x, int y) { // set current cursor position
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD getCurrentCursorPos() { // get current cursor position
    COORD curPos;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPos.X = curInfo.dwCursorPosition.X;
    curPos.Y = curInfo.dwCursorPosition.Y;

    return curPos;
}

void removeCursor() { // remove cursor blinking
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y, char* s) {
    if (stage5InPlay && !nearTheHead(x, y) && isIn(x, y) && !isFever()) return;

    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void gotoxycol(int x, int y, int col, char* s) {
    if (stage5InPlay && !nearTheHead(x, y) && isIn(x, y) && !isFever()) return;

    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
    printf("%s", s);
}

// 0(Black) 1(Dark Blue) 2(Dark Green) 3(Dark Sky Blue) 4(Dark Red)
// 5(Dark Purple) 6(Dark Yellow) 7(Gray) 8(Dark Gray) 9(Blue)
// 10(Green) 11(Sky Blue) 12(Red) 13(Purple) 14(Yellow) 15(White)


/*-----------------------------------------------------------------------------------------*/

void introScreen() {
    openingBGM(0);

    gotoxycol(8, 10, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 11, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 12, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 13, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 14, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 15, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 16, 14, "������������������������������������������������������������������������������������������������������������");
    gotoxycol(8, 17, 14, "������������������������������������������������������������������������������������������������������������");

    while (1) {
        if (kbhit()) break;
        gotoxycol(47, 22, 14, "[ Press any key to start ]");
        Sleep(700);
        gotoxycol(47, 22, 14, "                          ");
        Sleep(100);
    }
    //getch(); openingBGM�� �Է��� �ѹ� �޴°��� selectStage���� �ѹ� �����淡 �̰� ��
    openingBGM(1);
    system("cls");
}

void startScreen() {
    gotoxycol(48, 10, 14, "Press ENTER to choose mode: ");
    gotoxycol(53, 13, 14, "�� ��STORY");
    gotoxycol(54, 15, 14, "��TUTORIAL");
    gotoxycol(54, 17, 14, "��PLAY");
    int key = 0, count = 0;
    int mode = 0;

    while (1) {
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) {
                selectSound();
                mode = count;
            }
            else if (key == UP) count--;
            else if (key == DOWN) count++;
            if (count == 0) count = 1;
            if (count == 4) count = 1;

            switch (count) {
            case 1:
                gotoxycol(53, 13, 14, "�� ��STORY");
                gotoxycol(54, 15, 14, "��TUTORIAL     ");
                gotoxycol(54, 17, 14, "��PLAY     ");
                break;
            case 2:
                gotoxycol(53, 15, 14, "�� ��TUTORIAL");
                gotoxycol(54, 13, 14, "��STORY     ");
                gotoxycol(54, 17, 14, "��PLAY     ");
                break;
            case 3:
                gotoxycol(53, 17, 14, "�� ��PLAY     ");
                gotoxycol(54, 13, 14, "��STORY     ");
                gotoxycol(54, 15, 14, "��TUTORIAL     ");
                break;
            default:
                break;
            }
        }
        switch (mode) {
        case 1:
            gotoxycol(53, 13, 2, "�� ��STORY");
            Sleep(600);
            for (int i = 0; i < 7; i++) {
                gotoxycol(53, 13, 2, "�� ��STORY");
                Sleep(100);
                gotoxycol(53, 13, 15, "�� ��STORY");
                Sleep(100);
            }
            story();
            key = 0, count = 0, mode = 0;
            break;
        case 2:
            gotoxycol(53, 15, 2, "�� ��TUTORIAL");
            Sleep(600);
            for (int i = 0; i < 7; i++) {
                gotoxycol(53, 15, 2, "�� ��TUTORIAL");
                Sleep(100);
                gotoxycol(53, 15, 15, "�� ��TUTORIAL");
                Sleep(100);
            }
            tutorial();
            key = 0, count = 0, mode = 0;
            break;
        case 3:
            gotoxycol(53, 17, 2, "�� ��PLAY");
            Sleep(600);
            for (int i = 0; i < 7; i++) {
                gotoxycol(53, 17, 2, "�� ��PLAY");
                Sleep(100);
                gotoxycol(53, 17, 15, "�� ��PLAY");
                Sleep(100);
            }
            return;
        default:
            break;
        }
    }
}

void story() {
    system("cls");
    gotoxycol(37, 5, 10, "           /^|/^|");
    gotoxycol(37, 6, 10, "         _|_ _| | )");
    gotoxycol(49, 6, 15, "0  0");
    gotoxycol(37, 7, 10, "      /~     |_/    )");
    gotoxycol(37, 7, 12, "V");
    gotoxycol(37, 8, 10, "     |__________/    )");
    gotoxycol(37, 8, 12, "|____");
    gotoxycol(37, 9, 10, "        /_______      )");
    gotoxycol(37, 10, 10, "                `'/     )");
    gotoxycol(37, 11, 10, "                 /     /                    \\");
    gotoxycol(37, 12, 10, "               /     /                        \\\\");
    gotoxycol(37, 13, 10, "             /     /                            \\ \\");
    gotoxycol(37, 14, 10, "           /     /             _----_            \\  \\");
    gotoxycol(37, 15, 10, "          /     /           _-~      ~-_         |   |");
    gotoxycol(37, 16, 10, "         (      (        _-~    _--_    ~-_     _/   |");
    gotoxycol(37, 17, 10, "          (      ~-____-~    _-~    ~-_    ~-_-~    /");
    gotoxycol(37, 18, 10, "            ~-_           _-~          ~-_       _-~");
    gotoxycol(37, 19, 10, "               ~--______-~                ~-___-~");

    char story[105] = "������ ��, �ΰ��� �̹��Ⱑ �����ϴ� �ô밡 �־���. �׷��� �̹������ 21���� �Ǵ� �ؿ� �����ָ� ��� ����";
    char story1[105] = "�Ǿ� ��õ�ؾ� �ߴµ�, ���ΰ� '�̹���'�� �� Ư���� ���� �¾� �����ָ� ã�� ������ ������ �����Ѵ�. ������";
    char story2[50] = "�ٸ� �̹������� �޸� ��źġ���� �ʾ� ���̴µ���";
    char story3[57] = "...���� �̹���� ������ �����ָ� ��� ���� �� �� ������?";

    int i;

    setTextColor(14);
    setCurrentCursorPos(10, 23);
    typeSound();
    for (i = 0; i < 105; i++) {

        printf("%c", story[i]);
        Sleep(20);
    }
    setCurrentCursorPos(10, 25);
    typeSound();
    for (i = 0; i < 105; i++) {
        printf("%c", story1[i]);
        Sleep(20);
    }
    setCurrentCursorPos(10, 27);
    for (i = 0; i < 50; i++) {
        printf("%c", story2[i]);
        Sleep(20);
    }
    Sleep(2000);

    setCurrentCursorPos(10, 23);
    for (i = 0; i < 105; i++) printf("  ");
    setCurrentCursorPos(10, 25);
    for (i = 0; i < 105; i++) printf("  ");
    setCurrentCursorPos(10, 27);
    for (i = 0; i < 50; i++) {
        printf("  ");
    }
    typeSound();
    Sleep(40);
    setCurrentCursorPos(36, 25);
    for (int i = 0; i < 57; i++) {
        printf("%c", story3[i]);
        Sleep(70);
    }
    int key = 0;
    while (1) {
        if (kbhit()) {
            key = getch();
            if (key == GB) {
                system("cls");
                gotoxycol(48, 10, 14, "Press ENTER to choose mode: ");
                gotoxycol(53, 13, 14, "�� ��STORY");
                gotoxycol(54, 15, 14, "��TUTORIAL");
                gotoxycol(54, 17, 14, "��PLAY");
                break;
            }
        }
        gotoxycol(70, 29, 14, "[ Press X to go back ]");
        Sleep(700);
        gotoxy(70, 29, "                       ");
        Sleep(100);
    }
}

void tutorial() {
    system("cls");
    for (int y = 0; y <= SBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(SBOARD_ORIGIN_X, SBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == SBOARD_HEIGHT) gotoxycol(SBOARD_ORIGIN_X, SBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(SBOARD_ORIGIN_X, SBOARD_ORIGIN_Y + y, 14, "��");

        if (y == 0) gotoxycol(SBOARD_ORIGIN_X + (SBOARD_WIDTH + 1) * 2, SBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == SBOARD_HEIGHT) gotoxycol(SBOARD_ORIGIN_X + (SBOARD_WIDTH + 1) * 2, SBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(SBOARD_ORIGIN_X + (SBOARD_WIDTH + 1) * 2, SBOARD_ORIGIN_Y + y, 14, "��");
    }
    for (int x = 1; x < SBOARD_WIDTH + 1; x++) {
        gotoxycol(SBOARD_ORIGIN_X + x * 2, SBOARD_ORIGIN_Y, 14, "��");
        gotoxycol(SBOARD_ORIGIN_X + x * 2, SBOARD_ORIGIN_Y + SBOARD_HEIGHT, 14, "��");
    }
    for (int y = 0; y <= SCBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(SCBOARD_ORIGIN_X, SCBOARD_ORIGIN_Y + y, 15, "��");
        else if (y == SCBOARD_HEIGHT) gotoxycol(SCBOARD_ORIGIN_X, SCBOARD_ORIGIN_Y + y, 15, "��");
        else gotoxycol(SCBOARD_ORIGIN_X, SCBOARD_ORIGIN_Y + y, 15, "��");

        if (y == 0) gotoxycol(SCBOARD_ORIGIN_X + (SCBOARD_WIDTH + 1) * 2, SCBOARD_ORIGIN_Y + y, 15, "��");
        else if (y == SCBOARD_HEIGHT) gotoxycol(SCBOARD_ORIGIN_X + (SCBOARD_WIDTH + 1) * 2, SCBOARD_ORIGIN_Y + y, 15, "��");
        else gotoxycol(SCBOARD_ORIGIN_X + (SCBOARD_WIDTH + 1) * 2, SCBOARD_ORIGIN_Y + y, 15, "��");
    }
    for (int x = 1; x < SCBOARD_WIDTH + 1; x++) {
        gotoxycol(SCBOARD_ORIGIN_X + x * 2, SCBOARD_ORIGIN_Y, 15, "��");
        gotoxycol(SCBOARD_ORIGIN_X + x * 2, SCBOARD_ORIGIN_Y + SCBOARD_HEIGHT, 15, "��");
    }
    gotoxycol(43, 9, 8, "CONTROLS");
    gotoxycol(46, 11, 10, "��");
    gotoxycol(44, 12, 10, "��  ��");
    gotoxycol(46, 13, 10, "��");
    gotoxycol(39, 15, 7, "[ ] to use PET");
    gotoxycol(40, 15, 10, "R");
    gotoxycol(39, 16, 7, "[     ] to STOP");
    gotoxycol(40, 16, 10, "SPACE");

    gotoxycol(59, 9, 8, "ITEMS");
    gotoxycol(59, 11, 11, "�� : GOOD");
    gotoxycol(59, 13, 15, "���̡�, �ӵ���, �����");
    gotoxycol(59, 15, 15, "������ ��ź!, �� ��������");

    gotoxycol(59, 17, 12, "��: BAD");
    gotoxycol(59, 19, 15, "���̡�, �ӵ���");

    int key = 0;
    while (1) {
        if (kbhit()) {
            key = getch();
            if (key == GB) {
                system("cls");
                startScreen();
                break;
            }
        }
        gotoxycol(38, 22, 14, "[ Press X to go back ]");
        Sleep(700);
        gotoxy(38, 22, "                       ");
        Sleep(100);
    }
}

void selectStage() {
    getBestScore();
    gotoxycol(48, 8, 14, "Press stage number to play: ");
    gotoxycol(49, 11, 14, "�� ��Stage 1: Classic");
    if (stageBestScore[0] < TARGET_SCORE) gotoxycol(50, 13, 8, "��Stage 2: Off The Wall\t[ LOCKED ]");
    else gotoxycol(50, 13, 14, "��Stage 2: Off The Wall\t");
    if (stageBestScore[1] < TARGET_SCORE) gotoxycol(50, 15, 8, "��Stage 3: �̹�����    \t[ LOCKED ]");
    else gotoxycol(50, 15, 14, "��Stage 3: �̹�����\t");
    if (stageBestScore[2] < TARGET_SCORE) gotoxycol(50, 17, 8, "��Stage 4: �����̹��⵿\t[ LOCKED ]");
    else gotoxycol(50, 17, 14, "��Stage 4: �����̹��⵿\t");
    if (stageBestScore[3] < TARGET_SCORE) gotoxycol(50, 19, 8, "��Stage 5: ��¦�̹���  \t[ LOCKED ]");
    else  gotoxycol(50, 19, 14, "��Stage 5: ��¦�̹���\t");

    int key = 0, count = 0;
    stage = 0;

    while (1) {
        if (_kbhit()) {
            key = _getch();
            if (key == ENTER) {
                selectSound();
                stage = count;
            }
            else if (key == UP) {

                count--;
            }
            else if (key == DOWN) {

                count++;
            }
            if (stageBestScore[3] >= TARGET_SCORE) if (count == 0) count = 1;
            if (count == 0) count = 1;
            if (count == 6) count = 1;

            switch (count) {
            case 1:
                gotoxycol(49, 11, 14, "�� ��Stage 1: Classic");
                if (stageBestScore[0] < TARGET_SCORE) gotoxycol(50, 13, 8, "��Stage 2: Off The Wall\t[ LOCKED ]");
                else gotoxycol(50, 13, 14, "��Stage 2: Off The Wall\t");
                if (stageBestScore[3] < TARGET_SCORE) gotoxycol(50, 19, 8, "��Stage 5: ��¦�̹���  \t[ LOCKED ]");
                else  gotoxycol(50, 19, 14, "��Stage 5: ��¦�̹���\t");
                break;

            case 2:
                if (stageBestScore[0] < TARGET_SCORE) {
                    count = 1;
                    break;
                    gotoxycol(50, 13, 8, "�� ��Stage 2: Off The Wall\t[ LOCKED ]");
                }
                else gotoxycol(49, 13, 14, "�� ��Stage 2: Off The Wall\t");
                gotoxycol(50, 11, 14, "��Stage 1: Classic  ");
                if (stageBestScore[1] < TARGET_SCORE) gotoxycol(50, 15, 8, "��Stage 3: �̹�����    \t[ LOCKED ]");
                else gotoxycol(50, 15, 14, "��Stage 3: �̹�����\t");
                break;

            case 3:
                if (stageBestScore[1] < TARGET_SCORE) {
                    count--;
                    break;
                    gotoxycol(50, 15, 8, "�� ��Stage 3: �̹�����\t[ LOCKED ]");
                }
                else gotoxycol(49, 15, 14, "�� ��Stage 3: �̹�����\t");
                if (stageBestScore[0] < TARGET_SCORE) gotoxycol(50, 13, 8, "��Stage 2: Off The Wall\t[ LOCKED ]");
                else gotoxycol(50, 13, 14, "��Stage 2: Off The Wall\t");
                if (stageBestScore[2] < TARGET_SCORE) gotoxycol(50, 17, 8, "��Stage 4: �����̹��⵿\t[ LOCKED ]");
                else gotoxycol(50, 17, 14, "��Stage 4: �����̹��⵿\t");
                break;

            case 4:
                if (stageBestScore[2] < TARGET_SCORE) {
                    count--;
                    break;
                    gotoxycol(50, 17, 8, "�� ��Stage 4: �����̹��⵿\t[ LOCKED ]");
                }
                else gotoxycol(49, 17, 14, "�� ��Stage 4: �����̹��⵿\t");
                if (stageBestScore[1] < TARGET_SCORE) gotoxycol(50, 15, 8, "��Stage 3: �̹�����    \t[ LOCKED ]");
                else gotoxycol(50, 15, 14, "��Stage 3: �̹�����\t");
                if (stageBestScore[3] < TARGET_SCORE) gotoxycol(50, 19, 8, "��Stage 5: ��¦�̹��� \t[ LOCKED ]");
                else  gotoxycol(50, 19, 14, "��Stage 5: ��¦�̹���\t");
                break;
            case 5:
                if (stageBestScore[3] < TARGET_SCORE) {
                    if (stageBestScore[0] < TARGET_SCORE) {
                        count = 1;
                        break;
                    }
                    else {
                        count--;
                        break;
                    }
                    gotoxycol(50, 19, 8, "�� ��Stage 5: ��¦�̹���\t[ LOCKED ]");
                }
                else  gotoxycol(49, 19, 14, "�� ��Stage 5: ��¦�̹���\t");
                gotoxycol(50, 11, 14, "��Stage 1: Classic  ");
                if (stageBestScore[2] < TARGET_SCORE) gotoxycol(50, 17, 8, "��Stage 4: �����̹��⵿\t[ LOCKED ]");
                else gotoxycol(50, 17, 14, "��Stage 4: �����̹��⵿\t");
                break;

            default: break;
            }
        }
        switch (stage) {
        case 1:
            gotoxycol(49, 11, 2, "�� ��Stage 1: Classic");
            Sleep(600);
            for (int i = 0; i < 7; i++) {
                gotoxycol(49, 11, 2, "�� ��Stage 1: Classic");
                Sleep(100);
                gotoxycol(49, 11, 15, "�� ��Stage 1: Classic");
                Sleep(100);
            }
            break;

        case 2:
            if (stageBestScore[0] >= TARGET_SCORE) {
                gotoxycol(50, 13, 2, "�� ��Stage 2: Off The Wall");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 13, 2, "�� ��Stage 2: Off The Wall");
                    Sleep(100);
                    gotoxycol(50, 13, 15, "�� ��Stage 2: Off The Wall");
                    Sleep(100);
                }
                break;
            }
            else continue;

        case 3:
            if (stageBestScore[1] >= TARGET_SCORE) {
                gotoxycol(50, 15, 2, "�� ��Stage 3: �̹�����");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 15, 2, "�� ��Stage 3: �̹�����");
                    Sleep(100);
                    gotoxycol(50, 15, 15, "�� ��Stage 3: �̹�����");
                    Sleep(100);
                }
                break;
            }
            else continue;

        case 4:
            if (stageBestScore[2] >= TARGET_SCORE) {
                gotoxycol(50, 17, 2, "�� ��Stage 4: �����̹��⵿");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 17, 2, "�� ��Stage 4: �����̹��⵿");
                    Sleep(100);
                    gotoxycol(50, 17, 15, "�� ��Stage 4: �����̹��⵿");
                    Sleep(100);
                }
                break;
            }
            else continue;

        case 5:
            if (stageBestScore[3] >= TARGET_SCORE) {
                gotoxycol(50, 19, 14, "�� ��Stage 5: ��¦�̹���");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 19, 2, "�� ��Stage 5: ��¦�̹���");
                    Sleep(100);
                    gotoxycol(50, 19, 15, "�� ��Stage 5: ��¦�̹���");
                    Sleep(100);
                }
                break;
            }
            else continue;

        default: continue;
        }
        system("cls");
        break;
    }
}

void initGameBoardInfo() { // setting up boundaries
    for (int y = 0; y < GBOARD_HEIGHT; y++) {
        gameBoardInfo[y][0] = 1;
        gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
    }
    for (int x = 0; x < GBOARD_WIDTH + 2; x++) {
        gameBoardInfo[0][x] = 1;
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }
}

void resetGameBoardInfo() { // setting up boundaries
    for (int y = 1; y < GBOARD_HEIGHT; y++) {
        for (int x = 1; x < GBOARD_WIDTH; x++) {
            gameBoardInfo[y][x] = 0;
        }
    }
}

void drawBoard() {
    // game title
    gotoxycol(38, 2, 14, "SWITCH DRAGON");
    initGameBoardInfo();
    // draw game board
    for (int y = 0; y <= GBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "��");

        if (y == 0) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "��");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y, 14, "��");
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT, 14, "��");
    }
    // print ultimate
    drawUlt();
    drawFever();
    printStage();
    printScore();
    printHighscore();
    drawLife();
    drawControls();
    drawKeys();
}

void drawUlt() {
    // draw ultimate board
    for (int y = 0; y <= UBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "��");

        if (y == 0) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "��");
    }
    for (int x = 1; x < UBOARD_WIDTH + 1; x++) {
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y, 14, "��");
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y + UBOARD_HEIGHT, 14, "��");
    }
    // print ult
    gotoxycol(15, 25, 15, "PET ");
    gotoxycol(19, 25, 15, "������");
}

void drawFever() {
    // draw fever board
    for (int y = 0; y <= FBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "��");

        if (y == 0) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "��");
    }
    for (int x = 1; x < FBOARD_WIDTH + 1; x++) {
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y, 14, "��");
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y + FBOARD_HEIGHT, 14, "��");
    }
    // print fever
    gotoxycol(56, 25, 15, "F  E  V  E  R");
}

void printStage() {
    switch (stage) {
    case 1: gotoxycol(72, 5, 15, "Stage 1: Classic"); break;
    case 2: gotoxycol(72, 5, 15, "Stage 2: Off The Wall"); break;
    case 3: gotoxycol(72, 5, 15, "Stage 3: �̹�����"); break;
    case 4: gotoxycol(72, 5, 15, "Stage 4: �����̹��⵿"); break;
    case 5: gotoxycol(72, 5, 15, "Stage 5: ��¦�̹���"); break;
    default: break;
    }
}

void printHighscore() {
    gotoxycol(72, 7, 15, "Highscore: ");
    setCurrentCursorPos(83, 7);
    printf("%5d", stageBestScore[stage - 1]);
    //print Highscore after setting cursor position and calling from text file
}

void printScore() {
    gotoxycol(72, 9, 15, "Score: ");
    setCurrentCursorPos(83, 9);
    printf("%5d", currentScore);
    // print Score after setting cursor position
}

void drawLife() {
    gotoxycol(72, 11, 15, "Life:");
    gotoxycol(79, 11, 12, "");
    for (int i = 0; i < heart; i++) printf(" ��");
    for (int i = heart; i < 3; i++) printf(" ��");
}

void drawControls() {
    // draw fever board
    for (int y = 0; y <= CBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "��");

        if (y == 0) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "��");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "��");
        else gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "��");
    }
    for (int x = 1; x < CBOARD_WIDTH + 1; x++) {
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y, 14, "��");
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y + CBOARD_HEIGHT, 14, "��");
    }
}

void drawKeys() {
    gotoxycol(77, 14, 14, "CONTROLS");
    gotoxycol(80, 16, 7, "��");
    gotoxycol(78, 17, 7, "��  ��");
    gotoxycol(80, 18, 7, "��");
    gotoxycol(73, 20, 7, "[R] to use PET");
    gotoxycol(73, 21, 7, "[SPACE] to STOP");
}

void clearScreen() {
    for (int y = 1; y <= GBOARD_HEIGHT - 1; y++) {
        for (int x = 1; x <= GBOARD_WIDTH; x++) {
            gotoxy(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y, "  ");
        }
    }
}

void clearControls() {
    for (int y = 1; y <= CBOARD_HEIGHT - 1; y++) {
        for (int x = 1; x <= CBOARD_WIDTH; x++) {
            gotoxy(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y + y, "  ");
        }
    }
}

void ending() {
    char story[54] = "�Ϳ�! �̹��Ⱑ ��� ������ ����߱���. ������ҽ��ϴ�.";
    char story1[45] = "���� �̹���� \"��\"�� �Ǿ� ��õ�� �� �ְڱ���.";
    char story2[20] = "...? ��...? ���?";
    char story3[3] = "...";
    char story4[10] = "(������)";
    char story5[80] = "�̹��⿡�Դ� ����� ����� �־���. �״� ��� \"�̹���\"��� �̸��� ���� ���̾���.";
    char story6[75] = "������ �ڽ��� ����� ����� �˰� �� �̹���� �װ��� �β��������� �ʾҴ�.";
    char story7[105] = "������ �ڽ��� ���Ÿ� �޾Ƶ��̰�, �밨�԰� ���뼺�� ���� '��'���μ� �ڽ��� ���縦 �޾Ƶ鿴��. ";
    char story8[41] = "�̰��̾߸��� ������ ��õ�� ������ �𸥴�.";

    int i;

    setTextColor(14);
    setCurrentCursorPos(35, 11);
    typeSound();
    for (i = 0; i < 54; i++) {
        printf("%c", story[i]);
        Sleep(20);
    }
    setCurrentCursorPos(40, 13);
    for (i = 0; i < 45; i++) {
        printf("%c", story1[i]);
        Sleep(20);
    }
    Sleep(2000);
    setCurrentCursorPos(56, 17);
    for (i = 0; i < 11; i++) {
        printf("%c", story2[i]);
        Sleep(70);
    }
    Sleep(2000);
    setCurrentCursorPos(60, 19);
    for (i = 0; i < 3; i++) {
        printf("%c", story3[i]);
        Sleep(100);
    }
    Sleep(2000);
    setCurrentCursorPos(35, 11);
    for (i = 0; i < 54; i++) printf("  ");
    setCurrentCursorPos(40, 13);
    for (i = 0; i < 45; i++) printf("  ");
    setCurrentCursorPos(56, 17);
    for (i = 0; i < 11; i++) {
        printf("  ");
    }
    setCurrentCursorPos(60, 19);
    for (i = 0; i < 3; i++) {
        printf("  ");
    }
    Sleep(30);
    setCurrentCursorPos(57, 15);
    thunderSound();
    for (i = 0; i < 10; i++) {
        printf("%c", story4[i]);
        Sleep(70);
    }
    Sleep(5000);
    setCurrentCursorPos(57, 15);
    for (i = 0; i < 10; i++) {
        printf("  ");
    }
    setCurrentCursorPos(22, 11);
    typeSound();
    for (i = 0; i < 80; i++) {
        printf("%c", story5[i]);
        Sleep(20);
    }
    setCurrentCursorPos(26, 13);
    for (i = 0; i < 75; i++) {
        printf("%c", story6[i]);
        Sleep(20);
    }
    typeSound();
    setCurrentCursorPos(16, 17);
    for (i = 0; i < 105; i++) {
        printf("%c", story7[i]);
        Sleep(20);
    }
    Sleep(1000);
    setCurrentCursorPos(42, 19);
    for (i = 0; i < 41; i++) {
        printf("%c", story8[i]);
        Sleep(20);
    }
    Sleep(2000);
    int key = 0;

    while (1) {
        if (kbhit()) {
            key = getch();
            if (key == GB) {
                break;
            }
        }
        gotoxycol(70, 29, 14, "[ Press X to restart ]");
        Sleep(700);
        gotoxy(70, 29, "                       ");
        Sleep(100);
    }
}

void gameOver() {
    gameoverSound();

    Sleep(700);

    system("cls");
    gotoxycol(38, 7, 12, "������  ������  ���  ���  ������");
    gotoxycol(38, 8, 12, "��          ��      ��  ��  ��  ��  ��");
    gotoxycol(38, 9, 12, "��  ����  ������  ��  ��  ��  ������");
    gotoxycol(38, 10, 12, "��      ��  ��      ��  ��      ��  ��");
    gotoxycol(38, 11, 12, "������  ��      ��  ��      ��  ������");
    gotoxycol(38, 13, 12, "������  ��      ��  ������  �����");
    gotoxycol(38, 14, 12, "��      ��  ��      ��  ��          ��      ��");
    gotoxycol(38, 15, 12, "��      ��  ��      ��  ������  �����");
    gotoxycol(38, 16, 12, "��      ��    ��  ��    ��          ��      ��");
    gotoxycol(38, 17, 12, "������      ��      ������  ��      ��");

    gotoxycol(58, 20, 15, "SCORE ");
    // print final score
    gotoxycol(60, 22, 14, "");
    printf("%d", currentScore);
    gotoxycol(56, 24, 15, "HIGHSCORE ");
    gotoxycol(60, 26, 14, "");
    printf("%d", stageBestScore[stage - 1]);
    // print highscore

    while (1) {
        if (kbhit()) {
            break;
        }
        gotoxycol(47, 28, 14, "[ Press any key to restart ]");
        Sleep(700);
        gotoxycol(47, 28, 14, "                             ");
        Sleep(100);
    }
}

void stageClr() {
    clearSound();

    gotoxycol(31, 10, 14, "�������������� ���� ����");
    gotoxycol(31, 11, 14, "����  ��  ���� ���� �� ");
    gotoxycol(31, 12, 14, "����  ��  ���� ���� ����");
    gotoxycol(31, 13, 14, "���� ��  ���� ���� ����");
    gotoxycol(31, 14, 14, "��   ��  ��   ���� ����");
    gotoxycol(31, 15, 14, "���� �������� ���� ����");

    while (1) {
        if (kbhit()) {
            break;
        }
        gotoxycol(30, 17, 12, "[ Press ENTER to continue ]");
        Sleep(700);
        gotoxycol(30, 17, 15, "                            ");
        Sleep(100);
    }
}

/*----------------------------------------------------------------------------------------------*/

void initMoogi() {

    COORD headPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH , GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };
    COORD bodyPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH - 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };
    COORD tailPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH - 4, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };

    head = getNode(NULL, NULL, headPos);
    body = getNode(NULL, NULL, bodyPos);
    tail = getNode(NULL, NULL, tailPos);

    head->back = body;
    body->front = head;
    body->back = tail;
    tail->front = body;

    head->direct = RIGHT;
    body->direct = RIGHT;
    tail->direct = RIGHT;

    item = (Item*)malloc(sizeof(Item));
    item->itemNo = -1;

    int arrX = (head->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = head->position.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 2;
    gameBoardInfo[arrY][arrX - 1] = 2;
    gameBoardInfo[arrY][arrX - 2] = 2;

    if (stage == 3) {
        changeMoogiColor();
        changeMoogiBodyColor();
    }
    gotoxy(head->position.X, head->position.Y, "��");
    gotoxy(body->position.X, body->position.Y, "��");
    gotoxy(tail->position.X, tail->position.Y, "��");
}

void addBody() {
    if (isFever()) return;

    if (length + 1 > maxLength) return;

    length++;

    COORD newTailPos = { tail->position.X, tail->position.Y };

    switch (tail->direct) {
    case LEFT:
        newTailPos.X += 2; break;
    case RIGHT:
        newTailPos.X -= 2; break;
    case UP:
        newTailPos.Y++; break;
    case DOWN:
        newTailPos.Y--; break;
    default:
        break;
    }

    Moogi* p = getNode(tail, NULL, newTailPos);
    p->direct = tail->direct;
    tail->back = p;
    tail = p;

    if (!detectCollision(tail->position.X, tail->position.Y))
    {
        int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = tail->position.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 2;
    }
}

void deleteBody() {
    if (length > minLength) { // �ּұ��̺��� Ŭ ���� ����
        eraseTail();
        length--;
    }
}

void speedUp() {
    speed -= 5;
    if (speed < minSpeed) speed = minSpeed; // �ִ� �ӵ� ����
}

void speedDown() {
    speed += 5;
    if (speed > maxSpeed) speed = maxSpeed; // �ּ� �ӵ� ����
}

void createItem() {
    if (isFever()) return;

    if (itemCount != 0) return; // ������ ���� ����, ȭ�鿡 �������� ������ �������� ���� 
    time(&itemCreationTime);

    item->itemNo = rand() % 2 + 4; // ������ ���� 2����
    item->pos.X = rand() % GBOARD_WIDTH + GBOARD_ORIGIN_X;
    item->pos.Y = rand() % GBOARD_HEIGHT + GBOARD_ORIGIN_Y;

    do {
        item->pos.X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (item->pos.X % 2 == 1) item->pos.X++;
        item->pos.Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(item->pos.X, item->pos.Y)); // �ƹ��͵� ������ 0 ��ȯ��

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = item->itemNo;

    switch (item->itemNo) {
    case 4:
        setTextColor(1);
        gotoxy(item->pos.X, item->pos.Y, "��"); break;
    case 5:
        setTextColor(4);
        gotoxy(item->pos.X, item->pos.Y, "��"); break;
    default: break;
    }

    itemCount++;

    setTextColor(15);
}

void deleteItem() { // ������ ó�� or �������� ó��
    if (isFever()) return;

    time_t currentTime;
    time(&currentTime);

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);

    if ((int)difftime(currentTime, itemCreationTime) >= 7) {
        item->itemNo = -1;
        clearControls();
        drawKeys();
        gotoxy(item->pos.X, item->pos.Y, "  ");
        gameBoardInfo[arrY][arrX] = 0;
        itemCount--;
        createItem();
    }
}

void getHeart() {
    if (heart < 3) // ���� ���� ����... �׷��� ���� �ε����� ����. �ణ ��� ��� down..
        heart++;
    drawLife();
}

void getPet() {
    // ȭ�鿡 �� ǥ��
    // press r �� ���

    petGauge++;
    gotoxycol(19, 25, 2, "");
    for (int i = 0; i < petGauge; i++) printf("��");
    setTextColor(15);
    for (int i = petGauge; i < 5; i++) printf("��");
}

void usePet() {
    if (petGauge < 5) return;

    pet = (Pet*)malloc(sizeof(Pet));
    time(&petCreationTime);

    // ���� ������ ��ǥ, ��ǥ�� ��� �������� ��ǥ�� �̵�(�ִܰŸ���)
    pet->pos.X = tail->position.X;
    pet->pos.Y = tail->position.Y;
    eraseTail();
    //gotoxy(pet->pos.X, pet->pos.Y, "��"); // ���� : Ȯ�ο�, ��ġ Ȯ�� �ʿ�

    int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void petMove()
{
    if (isFever()) return;

    time_t currentTime;
    time(&currentTime);
    if ((int)difftime(currentTime, petCreationTime) >= 15)
    {
        /*if (stage5InPlay && turnOnAndOff == 1)
        {
            COORD pos = { pet->pos.X, pet->pos.Y };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
            printf("  ");
        }
        else */gotoxy(pet->pos.X, pet->pos.Y, "  ");
        int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
        int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 0;

        free(pet);
        pet = NULL;
        petGauge = 0;

        addBody();

        return;
    }

    int xPlus = pet->pos.X < item->pos.X ? 2 : (pet->pos.X == item->pos.X ? 0 : -2);
    int yPlus = pet->pos.Y < item->pos.Y ? 1 : (pet->pos.Y == item->pos.Y ? 0 : -1);

    int move = rand() % 2;
    COORD pos = { pet->pos.X, pet->pos.Y };

    if (move) pos.Y += yPlus;
    else pos.X += xPlus;

    if (pos.X == item->pos.X && pos.Y == item->pos.Y)
    {
        gotoxy(pet->pos.X, pet->pos.Y, "  ");
        pet->pos.X = pos.X;
        pet->pos.Y = pos.Y;
        gotoxycol(pet->pos.X, pet->pos.Y, 2, "��");
        setTextColor(15);

        getItemOfPet();

        gotoxy(pet->pos.X, pet->pos.Y, "  ");
        int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
        int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 0;

        free(pet);
        pet = NULL;
        petGauge = 0;

        gotoxycol(19, 25, 2, "");
        setTextColor(15);
        for (int i = 0; i < 5; i++) printf("��");

        addBody();

        return;
    }

    if (detectCollision(pos.X, pos.Y))
    {
        pos.X = pet->pos.X; pos.Y = pet->pos.Y;
        if (!move) pos.Y += yPlus;
        else pos.X += xPlus;
    }

    if (detectCollision(pos.X, pos.Y))
    {
        pos.X = pet->pos.X; pos.Y = pet->pos.Y;
        if (move) pos.Y -= yPlus;
        else pos.X -= xPlus;

        if (detectCollision(pos.X, pos.Y))
        {
            pos.X = pet->pos.X; pos.Y = pet->pos.Y;
            if (!move) pos.Y -= yPlus;
            else pos.X -= xPlus;
        }
    }

    gotoxycol(pet->pos.X, pet->pos.Y, 15, "  ");
    int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 0;

    pet->pos.X = pos.X;
    pet->pos.Y = pos.Y;
    gotoxycol(pet->pos.X, pet->pos.Y, 2, "��");
    setTextColor(15);

    arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void getItemOfPet() {
    itemCount--;
    getSound();
    if (item->itemNo == 4) { //����������(���̡�, �ӵ���, FEVER, �ǵ�, �����ֻ���)
        int itemType = (rand() % 4) + 1;//(rand() % 5) + 1; ���� �̰ǵ� ������ ������ 2����

        if (stage == 3 || stage == 4) {
            while (itemType == 4) itemType = (rand() % 5) + 1;
        }

        switch (itemType) {
        case 1: // ���̡�
            deleteBody();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 1, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 2, "���� ����!");
            break;

        case 2: // �ӵ���
            speedDown();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 1, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 2, "�ӵ� ����!");
            break;

        case 3:
            getHeart();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 12, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 2, "���� + 1"); // ����: ��Ʈ, ���� ��Ʈ�� ������ �� �� ����
            break;

        case 4:
            dragonBallBomb();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 1, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 2, "������ ��ź"); //���� : ��Ʈ
            break;

        default: break;
        }
        createItem();
    }
    else if (item->itemNo == 5) { // ������������ (���̡�, �ӵ���)
        int itemType = (rand() % 2) + 1;

        switch (itemType) {
        case 1: // ���̡�
            addBody();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 12, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 12, "���� ����!");
            break;
        case 2: // �ӵ���
            speedUp();
            clearControls();
            gotoxycol(77, 14, 14, "ITEM GET");
            gotoxycol(78, 17, 14, "[");
            gotoxycol(80, 17, 12, "��");
            gotoxycol(82, 17, 14, " ]");
            gotoxycol(76, 20, 12, "�ӵ� ����!");
            break;
        default: break;
        }

        createItem();
    }
}

void dragonBallBomb() {
    int multipleDragonBall = rand() % 3 + 2; // 2~4
    for (int i = 0; i < multipleDragonBall; i++) {
        addDragonBall();
    }
}

void moogiSwitch() {
    if (isFever()) return;

    Moogi* p = tail, * pp = p;
    Moogi* pH = NULL, * pT = NULL;
    Moogi* pi = NULL, * pii = NULL;

    while (p != NULL)
    {
        if (pii != NULL)
        {
            pi = getNode(pii, NULL, p->position);
            pii->back = pi;
        }
        else pi = getNode(NULL, NULL, p->position);

        pii = pi;
        pp = p;

        if (p == tail) pH = pi;

        switch (p->direct)
        {
        case UP:
            pi->direct = DOWN;
            break;
        case DOWN:
            pi->direct = UP;
            break;
        case LEFT:
            pi->direct = RIGHT;
            break;
        case RIGHT:
            pi->direct = LEFT;
            break;
        default:
            break;
        }

        p = p->front;
        pi = pi->back;

        free(pp);
    }
    pT = pii;

    head = pH;
    tail = pT;

    gotoxy(head->position.X, head->position.Y, "��");
    Moogi* tmp = head->back;
    while (tmp != NULL)
    {
        gotoxy(tmp->position.X, tmp->position.Y, "��");
        tmp = tmp->back;
    }

    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;

    switch (head->direct)
    {
    case UP:
        direction = 0; break;
    case DOWN:
        direction = 1; break;
    case LEFT:
        direction = 2; break;
    case RIGHT:
        direction = 3; break;
    }

    Sleep(100);
}

/*----------------------------------------------------------------------------------------------*/

void inPlayKeyInput() // �÷��� ���� Ű �Է�
// �̹��� ���� �� �Ͻ�����
{
    int i, key; // , isDone = 0;
    int s = speed; if (stage == 5) s *= 2;
    for (i = 0; i < s; i++)
    {
        if (_kbhit() != 0)
        {
            key = _getch();
            switch (key)
            {
            case UP:
                shiftUp();
                break;
            case DOWN:
                shiftDown();
                break;
            case LEFT:
                shiftLeft();
                break;
            case RIGHT:
                shiftRight();
                break;
            case SPACE:
                pausePlay();
                break;
            case R:
                usePet();
                break;
            default:
                break;
            }

        }

        Sleep(15); // �÷��� �ӵ� ����
    }
}

void shiftUp() // �÷��� �� up ����Ű �Է� ��
{
    // �浹 Ȯ��, ����ġ ���� ���� ��ȯ���� Ȯ��
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    // ���� ������Ʈ, �̵��� ���� head �� tail�� redraw
    direction = 0;
    drawHead(nextPos);
    eraseTail();
    showTurnOnArea();
}

void shiftDown() // �÷��� �� down ����Ű �Է� ��
{
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 1;
    drawHead(nextPos);
    eraseTail();
    showTurnOnArea();
}

void shiftLeft() // �÷��� �� left ����Ű �Է� ��
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 2;
    drawHead(nextPos);
    eraseTail();
    showTurnOnArea();
}

void shiftRight() // �÷��� �� right ����Ű �Է� ��
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 3;
    drawHead(nextPos);
    eraseTail();
    showTurnOnArea();
}

void pausePlay() // �Ͻ����� -> ����� Ȥ�� ������ ��ٸ�
// ȭ���� ��� �����Ǵ��� ����
// �ϴ� �Ʒ��� ����Ű ������ Ȩȭ������ ���ư���
// ���� ����Ű ������ �׳� �̾ ���� ����
// �����̽� ������ ���� Ȯ������ �س���
{
    while (1)
    {
        if (_kbhit() != 0)
        {
            int key = _getch();

            if (key == SPACE) break;
        }
        Sleep(speed);
    }
}

int detectCollision(int posX, int posY) // �浹 ����
// �� �Լ��� �Է� ���ڷ� �̹��� �Ӹ� ��ġ�� �־��ָ� ��
// �浹 �����ϰ� ������ direction�� ���� ��ĭ ������ ��ġ
// �׳� nextHeadPos() ���� �־��ָ� ��
{
    int arrX = (posX - GBOARD_ORIGIN_X) / 2;
    int arrY = (posY - GBOARD_ORIGIN_Y);

    return gameBoardInfo[arrY][arrX];
}

COORD nextHeadPos() // head�� ���� ��ġ ��ȯ
{
    COORD curPos;
    curPos.X = head->position.X;
    curPos.Y = head->position.Y;

    switch (direction)
    {
    case 0:
        curPos.Y--;
        break;
    case 1:
        curPos.Y++;
        break;
    case 2:
        curPos.X -= 2;
        break;
    case 3:
        curPos.X += 2;
        break;
    default:
        break;
    }

    return curPos;
}

void drawHead(COORD headPos) // �̹����� �̵��� ���(head)
// �̹��� ���Ḯ��Ʈ �տ� ��� �ϳ� �߰��ϰ�
// �Ӹ��� �� �ٷ� �� �� ĭ(���� ��ġ�� ��µ� �Ӹ��ݸ� ����� ����)�� ���
{
    Moogi* node = getNode(NULL, head, headPos);
    head->front = node;
    head = head->front;

    switch (direction)
    {
    case 0:
        head->direct = UP;
        break;
    case 1:
        head->direct = DOWN;
        break;
    case 2:
        head->direct = LEFT;
        break;
    case 3:
        head->direct = RIGHT;
        break;
    default:
        head->direct = UP;
        break;
    }

    if (stage == 3) {
        setTextColor_rygbw(moogiColor);
    }
    else setTextColor(15);

    if (stage == 4 && detectCollision(head->position.X, head->position.Y) == 3)
    {
        gotoxy(head->position.X, head->position.Y, "��");
        gotoxy(head->back->position.X, head->back->position.Y, "��");
    }
    else
    {
        COORD headPos = { head->position.X, head->position.Y };
        COORD headNextPos = { head->back->position.X, head->back->position.Y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), headPos); printf("��");
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), headNextPos); printf("��");

        //gotoxy(head->position.X, head->position.Y, "��");
        //gotoxy(head->back->position.X, head->back->position.Y, "��");
    }

    getSomething();

    int arrX = (headPos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (headPos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;
}

void eraseTail() // �̹����� �̵��� ���(tail)
// ������ �� ĭ ������ �̵���Ű�� ������ ��带 ����(drawHead���� �ϳ� �߰��ϴϱ� length�� �Ȱ���)
// �׸��� ������ �ڸ��� ���� ����� ���鹮�ڷ� ����
{
    if (tail == NULL) return;

    if (detectCollision(tail->position.X, tail->position.Y) == 2)
    {
        int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;


        COORD pos = { tail->position.X, tail->position.Y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        printf("  ");
    }

    Moogi* pi = tail;
    tail = tail->front;
    tail->back = NULL;
    free(pi);
}

Moogi* getNode(Moogi* front, Moogi* back, COORD position) // �̹��� ��� �ϳ� ����
{
    Moogi* node = (Moogi*)malloc(sizeof(Moogi));
    node->front = front;
    node->back = back;
    node->position = position;

    return node;
}

int moogiMove()
{
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2 || humanAttack)
    {
        if (heart > 0) { // ��� ������ ��
            collisionSound();
            waitToRecover();
            humanAttack = 0;
            return 1;
        }
        else return 0;
    }
    //gotoxy(0, 0, "                  ");

    if (detectCollision(nextPos.X, nextPos.Y) != 13)
    {
        drawHead(nextPos);      //�갡 getSomething �θ�, getSomething�� countScore�� getItem �θ�,
        eraseTail();
    }

    if (stage == 2)
    {
        humanMove();
        isHumanAttack();
    }

    if (pet != NULL) petMove();

    if (isFever())
    {
        fillFever();
        time_t currentFeverTime;
        time(&currentFeverTime);

        if ((int)difftime(currentFeverTime, feverStartTime) >= 10)
            removeFever();
    }

    return 1;
}

void getSomething()
{
    deleteItem();
    deleteFeverWord();
    countScore();
    getItem();
    getFever();
}

void humanMove()
{
    if (isFever()) return;

    Human* p = humanHead;

    while (p->next != NULL)
    {
        p = p->next;

        COORD pos = { p->position.X, p->position.Y };

        int xPlus = p->position.X < head->back->back->position.X ? 2 : (p->position.X == head->back->back->position.X ? 0 : -2);
        int yPlus = p->position.Y < head->back->back->position.Y ? 1 : (p->position.Y == head->back->back->position.Y ? 0 : -1);

        int cantMove = 4;

        while (1)
        {
            pos.X = p->position.X;
            pos.Y = p->position.Y;

            switch (rand() % 2)
            {
            case 0:
                pos.Y += yPlus;
                break;
            case 1:
                pos.X += xPlus;
                break;
            default:
                break;
            }

            if (!detectCollision(pos.X, pos.Y)) break;

            p->direct = rand() % 4;
            cantMove--;

            if (!cantMove) break;
        }

        if (!cantMove) continue;

        int arrX = (p->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (p->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;

        arrX = (pos.X - GBOARD_ORIGIN_X) / 2;
        arrY = (pos.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 1;

        setTextColor(3);
        gotoxy(p->position.X, p->position.Y, "  ");
        gotoxy(pos.X, pos.Y, "&/");
        setTextColor(15);

        p->position.X = pos.X;
        p->position.Y = pos.Y;

    }
}

void isHumanAttack()
{
    if (isFever()) return;

    Human* p = humanHead;

    while (p->next != NULL)
    {
        p = p->next;

        COORD headPos = { head->position.X, head->position.Y };
        COORD humanPos = { p->position.X, p->position.Y };

        if (headPos.X == humanPos.X && headPos.Y - 1 == humanPos.Y) humanAttack = 1;
        if (headPos.X == humanPos.X && headPos.Y + 1 == humanPos.Y) humanAttack = 1;
        if (headPos.X - 2 == humanPos.X && headPos.Y == humanPos.Y) humanAttack = 1;
        if (headPos.X + 2 == humanPos.X && headPos.Y == humanPos.Y) humanAttack = 1;
    }
}

/*-------------------------------------------------------------------------------------------*/

void countScore() {
    if (detectCollision(head->position.X, head->position.Y) == 3) {
        if (dragonBallCount > 0) // ������ ������ ���� �ʰ�
            dragonBallCount--;
        currentScore += 10;
        getSound();

        if (stage == 5)
        {
            for (int i = 0; i < stage5DragonBallNum; i++)
            {
                if (head->position.X == stage5DragonBallPos[i].X
                    && head->position.Y == stage5DragonBallPos[i].Y)
                {
                    for (int j = i; j < stage5DragonBallNum - 1; j++)
                        stage5DragonBallPos[j] = stage5DragonBallPos[j + 1];
                    stage5DragonBallNum--;
                    break;
                }
            }
        }

        if (currentScore > 0 && currentScore > feverScore)
        {
            feverScore += 10;
            createFever();
        }

        if (stage < 4) // �������� 4, 5�� ����ġ ��� off
            moogiSwitch();

        addBody();

        if (stage == 3)
        {
            changeMoogiColor();
            changeMoogiBodyColor();
        }

        if (stage == 4)
        {
            wormhole();
        }

        if (dragonBallCount == 0) { // �����ְ� ������ �߰��� �������� ����
            addDragonBall();
        }
        if (stage == 2) addHuman();

        if (currentScore > 0 && currentScore > speedScore) {
            speedScore += 50;
            speedUp();
        }
    }
    else if (detectCollision(head->position.X, head->position.Y) == 12) {
        currentScore += 3;
        getSound();
    }

    printScore();
}

void getBestScore() {    //���� ���� �� �÷��� ���� �ݵ�� �ҷ��� �Ѵ�...(1����)
    FILE* fp_r = fopen("BestScore.txt", "r");
    if (fp_r == NULL) {
        return;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(fp_r, "%d", &stageBestScore[i]);
    }
    fclose(fp_r);        //���Ͽ��� �ְ����� ���� �� ���� ���� stageBestScore�迭�� ����
}

void setBestScore(int score) {            //���ӿ����������� �θ��� �� ��?
    if (score > stageBestScore[stage - 1]) {
        stageBestScore[stage - 1] = score;
    }
}

void saveBestScore() {    //���� ���� �� �ٽ� �ְ������� ���Ͽ� �����ϴ� ����
    FILE* fp_w = fopen("BestScore.txt", "w");
    for (int i = 0; i < 5; i++) {
        fprintf(fp_w, "%d\n", stageBestScore[i]);
    }
    fclose(fp_w);
}

int isGameOver() {        //���ϰ��� 1�̳� 2�� ���������ؾߵ�
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2 || humanAttack)
        return 1;        //������ 1����
    return 0;            //�ƴϸ� 0
}

void getItem() {
    if (isFever()) return;

    if (detectCollision(head->position.X, head->position.Y) == 4 || detectCollision(head->position.X, head->position.Y) == 5) {
        itemCount--;
        getSound();
        if (item->itemNo == 4) {    //����������(���̡�, �ӵ���, FEVER, �ǵ�, �����ֻ���)
            int itemType = (rand() % 5) + 1;//(rand() % 5) + 1; ���� �̰ǵ� ������ ������ 2����

            if (stage == 3 || stage == 4) {
                while (itemType == 4) itemType = (rand() % 5) + 1;
            }
            if (stage == 5 && itemType == 5)
                itemType = (rand() % 4) + 1;

            switch (itemType) {
            case 1: //���̡�
                deleteBody();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 1, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(76, 20, 2, "���� ����!");
                break;

            case 2: //�ӵ���
                speedDown();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 1, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(76, 20, 2, "�ӵ� ����!");
                break;

            case 3:
                getHeart();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 12, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(77, 20, 2, "���� + 1"); // ����: ��Ʈ, ���� ��Ʈ�� ������ �� �� ����
                break;

            case 4:
                dragonBallBomb();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 1, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(76, 20, 2, "������ ��ź"); // ����: ��Ʈ
                break;

            case 5:
                getPet();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 15, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(77, 20, 2, "�� ȹ��"); // ����: ��Ʈ
                break;

            default: break;
            }
            createItem();
        }
        else if (item->itemNo == 5) { // ������������ (���̡�, �ӵ���)
            int itemType = (rand() % 2) + 1;

            switch (itemType) {
            case 1: // ���̡�
                addBody();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 12, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(76, 20, 12, "���� ����!");
                break;

            case 2: // �ӵ���
                speedUp();
                clearControls();
                gotoxycol(77, 14, 14, "ITEM GET");
                gotoxycol(78, 17, 14, "[");
                gotoxycol(80, 17, 12, "��");
                gotoxycol(82, 17, 14, " ]");
                gotoxycol(76, 20, 12, "�ӵ� ����!");
                break;

            default: break;
            }
            createItem();
        }
    }
    //0 �ƹ��͵����°�, 1 ����ֹ�, 2 �̹����, 3 ������, 4 ����������, 5 ���۾�����,
}

void addDragonBall() {
    int ballX;
    int ballY;

    if (stage == 3) {
        deleteDragonBall();
        for (int i = 0; i < 5; i++) {
            do {
                ballX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (ballX % 2 == 1) ballX++;
                ballY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
            } while (detectCollision(ballX, ballY));

            int arrX = (ballX - GBOARD_ORIGIN_X) / 2;
            int arrY = (ballY - GBOARD_ORIGIN_Y);
            if (moogiColor == dragonBallColor[i])
                gameBoardInfo[arrY][arrX] = 3;
            else
                gameBoardInfo[arrY][arrX] = 1;
            dragonBallPos[i].X = ballX;
            dragonBallPos[i].Y = ballY;
            showColorDragonBall(ballX, ballY, dragonBallColor[i]);
        }
        Sleep(100);
    }
    else if (stage == 4)
    {
        do {
            wormholePos[0].X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wormholePos[0].X % 2 == 1) wormholePos[0].X++;
            wormholePos[0].Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

            wormholePos[1].X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wormholePos[1].X % 2 == 1) wormholePos[1].X++;
            wormholePos[1].Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
        } while (detectCollision(wormholePos[0].X, wormholePos[0].Y)
            || detectCollision(wormholePos[1].X, wormholePos[1].Y)
            || (wormholePos[0].X == wormholePos[1].X) || (wormholePos[0].Y == wormholePos[1].Y));

        gameBoardInfo[(wormholePos[0].Y - GBOARD_ORIGIN_Y)][(wormholePos[0].X - GBOARD_ORIGIN_X) / 2] = 3;
        gameBoardInfo[(wormholePos[1].Y - GBOARD_ORIGIN_Y)][(wormholePos[1].X - GBOARD_ORIGIN_X) / 2] = 3;

        showDragonBall(wormholePos[0].X, wormholePos[0].Y);
        showDragonBall(wormholePos[1].X, wormholePos[1].Y);
    }
    else {
        do {
            ballX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (ballX % 2 == 1) ballX++;
            ballY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
        } while (detectCollision(ballX, ballY));

        int arrX = (ballX - GBOARD_ORIGIN_X) / 2;
        int arrY = (ballY - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 3;
        showDragonBall(ballX, ballY); dragonBallCount++;

        if (stage == 5)
        {
            stage5DragonBallPos[stage5DragonBallNum].X = ballX;
            stage5DragonBallPos[stage5DragonBallNum].Y = ballY;
            time(&stage5ShowDragonBall[stage5DragonBallNum]);
            stage5DragonBallNum++;
        }
    }
}

void showDragonBall(int x, int y) {
    setTextColor(6);
    gotoxy(x, y, "��");
    setTextColor(15);
}

// ȣ�� : ������ ������ ����
// �̹� �� �Ǵ� �������� ��µ� ���� ����ϸ� X
void addHuman() {
    if (dragonBallCount > 1) return; // �� ���� ����(���� ����)

    if (rand() % 3)
    {
        addBlock();
        return;
    }

    if (humanNum >= 3)
    {
        addBlock();
        return;
    }

    int humanX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int humanY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        humanX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (humanX % 2 == 1) humanX++;
        humanY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(humanX, humanY)); // �ƹ��͵� ������ 0 ��ȯ��

    setTextColor(14);
    gotoxy(humanX, humanY, "��");
    setTextColor(15);

    int arrX = (humanX - GBOARD_ORIGIN_X) / 2;
    int arrY = (humanY - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 1; // �� 1

    Human* node = (Human*)malloc(sizeof(Human));
    node->direct = rand() % 4;
    node->position.X = humanX;
    node->position.Y = humanY;
    node->next = NULL;

    Human* p = humanHead;
    while (p->next != NULL) p = p->next;
    p->next = node;

    humanNum++;
}

void addBlock()
{
    int humanX, humanY;
    int blockID = rand() % 6;

    while (1)
    {
        humanX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (humanX % 2 == 1) humanX++;
        humanY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

        if (humanX + 12 < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2
            && humanY + 6 < GBOARD_ORIGIN_Y + GBOARD_HEIGHT)
        {
            int empty = 1;

            for (int y = humanY; y < humanY + 4; y++)
            {
                for (int x = humanX; x < humanX + 8; x += 2)
                {
                    if (detectCollision(x, y) && blockModel[blockID][y - humanY][(x - humanX) / 2])
                    {
                        empty = 0;
                        break;
                    }
                }
                if (!empty) break;
            }

            if (empty)
            {
                for (int y = humanY; y < humanY + 4; y++)
                {
                    for (int x = humanX; x < humanX + 8; x += 2)
                    {
                        if (blockModel[blockID][y - humanY][(x - humanX) / 2])
                        {
                            int arrX = (x - GBOARD_ORIGIN_X) / 2;
                            int arrY = (y - GBOARD_ORIGIN_Y);

                            gameBoardInfo[arrY][arrX] = 1;
                            gotoxycol(x, y, 14, "��");
                            setTextColor(15);
                        }
                    }
                }

                break;
            }
        }
    }
}

//function for stage3
void setTextColor_rygbw(int colorNum) {
    switch (colorNum) {
    case 0: colorNum = 4; break;
    case 1: colorNum = 6; break;
    case 2: colorNum = 2; break;
    case 3: colorNum = 9; break;
    case 4: colorNum = 15; break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void changeMoogiColor() {      //�̹��� ���� ����, 0�� 1�� 2�� 3�� 4��
    moogiColor = rand() % 5;
}
void changeMoogiBodyColor() {   //�̹��� �� ���� �ٲ�(countScore���� �Ҹ�)
    setTextColor_rygbw(moogiColor);
    gotoxy(head->position.X, head->position.Y, "��");
    Moogi* p = head->back;
    while (p->back != NULL) {
        gotoxy(p->position.X, p->position.Y, "��");
        p = p->back;
    }
}

void setDragonBallColor() {     //�� ���� ȣ��, �ʱ�ȭ�뵵
    for (int i = 0; i < 5; i++)
        dragonBallColor[i] = i;     //0�� 1�� 2�� 3�� 4��
}

void setDragonBallPos() {       //�� ���� ȣ��, �ʱ�ȭ�뵵.
    for (int i = 0; i < 5; i++) {
        dragonBallPos[i].X = 0;
        dragonBallPos[i].Y = 0;
    }
}

void showColorDragonBall(int x, int y, int color) {
    setTextColor_rygbw(color);  //0�� 1�� 2�� 3�� 4��
    gotoxy(x, y, "��");
    setTextColor(15);
}

void deleteDragonBall() {       //������ ���� �� ���� �� �ƴ� ������ ����� �뵵(addDragonBall���� �Ҹ�)
    for (int i = 0; i < 5; i++) {
        if (dragonBallPos[i].X != 0 && dragonBallPos[i].Y != 0) {
            int arrX = (dragonBallPos[i].X - GBOARD_ORIGIN_X) / 2;
            int arrY = (dragonBallPos[i].Y - GBOARD_ORIGIN_Y);
            if (gameBoardInfo[arrY][arrX] == 1) {
                gameBoardInfo[arrY][arrX] = 0;
                gotoxy(dragonBallPos[i].X, dragonBallPos[i].Y, "  ");
            }
            dragonBallPos[i].X = 0;
            dragonBallPos[i].Y = 0;
        }
    }


}

void waitToRecover() {
    int key, flag = 0;
    gotoxycol(72, 23, 4, "PRESS TO CONTINUE!");
    Moogi* p = head->back;
    COORD nextPos;

    speedDown();

    for (int i = 0; i < 5; i++) {
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "��");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "��");
            if (p->back == NULL) break;
            p = p->back;
        }
        Sleep(300);
        p = head->back;
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "��");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "��");
            if (p->back == NULL) break;
            p = p->back;
        }
        Sleep(100);
        for (int i = 0; i < speed; i++)
        {
            if (_kbhit() != 0)
            {
                key = _getch();

                if (key == UP)
                {
                    if (direction == 0 || direction == 1) continue;
                    direction = 0;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == DOWN)
                {
                    if (direction == 0 || direction == 1) continue;
                    direction = 1;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == LEFT)
                {
                    if (direction == 2 || direction == 3) continue;
                    direction = 2;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == RIGHT)
                {
                    if (direction == 2 || direction == 3) continue;
                    direction = 3;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == SPACE)
                {
                    pausePlay();
                }
            }

            Sleep(15); // �÷��� �ӵ� ����
        }
        p = head->back;
        if (flag == 1) break;
    }

    heart--;
    drawLife();
    gotoxycol(72, 23, 14, "                   ");
    gotoxy(GBOARD_WIDTH + 3, GBOARD_HEIGHT + 5, "");
    setTextColor(15);

    if (flag == 0) {
        return;
    }
}

void createFever() { // FEVER ������ ����
    if (isFever()) return;
    if (feverWordPos.X) return;

    int ch = rand() % 5;

    int feverX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int feverY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        feverX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (feverX % 2 == 1) feverX++;
        feverY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(feverX, feverY));

    int arrX = (feverX - GBOARD_ORIGIN_X) / 2;
    int arrY = feverY - GBOARD_ORIGIN_Y;

    switch (ch)
    {
    case 0:
        gotoxycol(feverX, feverY, 10, "F");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 7;
        //fever[0] = 2;
        break;
    case 1:
        gotoxycol(feverX, feverY, 11, "E");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 8;
        //fever[1] = 2;
        break;
    case 2:
        gotoxycol(feverX, feverY, 12, "V");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 9;
        //fever[2] = 2;
        break;
    case 3:
        gotoxycol(feverX, feverY, 13, "E");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 10;
        //fever[3] = 2;
        break;
    case 4:
        gotoxycol(feverX, feverY, 9, "R");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 11;
        //fever[4] = 2;
        break;
    }

    time(&feverWordCreateTime);
    feverWordPos.X = feverX;
    feverWordPos.Y = feverY;
}

void deleteFeverWord()
{
    if (feverWordPos.X == 0) return;

    time_t feverWordDeleteTime;
    time(&feverWordDeleteTime);

    if ((int)difftime(feverWordDeleteTime, feverWordCreateTime) >= 7)
    {
        gameBoardInfo[feverWordPos.Y - GBOARD_ORIGIN_Y][(feverWordPos.X - GBOARD_ORIGIN_X) / 2] = 0;
        gotoxy(feverWordPos.X, feverWordPos.Y, "  ");
        feverWordPos.X = 0;
    }
}

void getFever() { // ȹ���ϸ� FEVER �迭 �ʱ�ȭ, GBOARD �Ʒ� FEVER ��Ȳ ������Ʈ
    int getIdx = detectCollision(head->position.X, head->position.Y);

    switch (getIdx)
    {
    case 7:
    {
        if (fever[0] == 1)
        {
            fever[0] = 0;
            gotoxycol(56, 25, 15, "F");
        }
        else
        {
            fever[0] = 1;
            gotoxycol(56, 25, 10, "F");
            setTextColor(15);
        }
        getSound();
        feverScore = currentScore + 20;
        if (isFever()) feverStart = 1;
        feverWordPos.X = 0;

        break;
    }
    case 8:
    {
        if (fever[1] == 1)
        {
            fever[1] = 0;
            gotoxycol(59, 25, 15, "E");
        }
        else
        {
            fever[1] = 1;
            gotoxycol(59, 25, 11, "E");
            setTextColor(15);
        }
        getSound();
        feverScore = currentScore + 20;
        if (isFever()) feverStart = 1;
        feverWordPos.X = 0;

        break;
    }
    case 9:
    {
        if (fever[2] == 1)
        {
            fever[2] = 0;
            gotoxycol(62, 25, 15, "V");
        }
        else
        {
            fever[2] = 1;
            gotoxycol(62, 25, 12, "V");
            setTextColor(15);
        }
        getSound();
        feverScore = currentScore + 20;
        if (isFever()) feverStart = 1;
        feverWordPos.X = 0;

        break;
    }
    case 10:
    {
        if (fever[3] == 1)
        {
            fever[3] = 0;
            gotoxycol(65, 25, 15, "E");
        }
        else
        {
            fever[3] = 1;
            gotoxycol(65, 25, 13, "E");
            setTextColor(15);
        }
        getSound();
        feverScore = currentScore + 20;
        if (isFever()) feverStart = 1;
        feverWordPos.X = 0;

        break;
    }
    case 11:
    {
        if (fever[4] == 1)
        {
            fever[4] = 0;
            gotoxycol(68, 25, 15, "R");
        }
        else
        {
            fever[4] = 1;
            gotoxycol(68, 25, 9, "R");
            setTextColor(15);
        }
        getSound();
        feverScore = currentScore + 20;
        if (isFever()) feverStart = 1;
        feverWordPos.X = 0;

        break;
    }
    default:
        break;
    }
}

int isFever() {
    if (fever[0] && fever[1] && fever[2] && fever[3] && fever[4]) return 1;
    else return 0;
}

void fillFever() {
    if (!feverStart) return;

    if (stage == 5) stage5InPlay = 0;

    getSound();

    feverStart = 0;
    time(&feverStartTime);

    if (stage == 2)
    {
        Human* p = humanHead, * pi = p;
        while (p != NULL)
        {
            p = p->next;
            free(pi);
            pi = p;
        }
        humanHead = (Human*)malloc(sizeof(Human));
        humanHead->next = NULL;
    }

    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 1, 11, "�ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 2, 11, "�ۡۡۡۡۡۡۡۡܡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 3, 11, "�ۡۡۡۡۡۡۡۡܡܡۡۡۡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 4, 11, "�ۡۡۡۡۡۡۡܡܡܡۡۡۡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 5, 11, "�ۡۡۡۡۡۡۡܡۡۡܡۡۡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 6, 11, "�ۡۡۡۡۡۡܡܡܡܡܡۡۡۡܡܡܡܡܡܡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 7, 11, "�ۡۡۡۡۡۡܡۡۡۡܡܡۡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 8, 11, "�ۡۡۡۡۡܡܡۡۡۡۡܡۡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 9, 11, "�ۡۡۡۡۡܡܡۡۡۡܡܡܡۡۡۡۡܡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 10, 11, "�ۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 11, 11, "�ۡۡۡۡۡۡۡۡܡܡۡۡۡܡܡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 12, 11, "�ۡۡۡۡۡۡۡܡܡܡܡܡܡܡܡܡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 13, 11, "�ۡۡۡۡۡۡۡܡܡܡܡܡܡܡܡܡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 14, 11, "�ۡۡۡۡۡۡۡۡܡܡܡܡܡܡܡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 15, 11, "�ۡۡۡۡۡۡۡۡۡܡܡܡܡܡۡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 16, 11, "�ۡۡۡۡۡۡۡۡۡۡܡܡܡۡۡۡۡۡۡۡۡۡۡۡۡ�");
    gotoxycol(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + 17, 11, "�ۡۡۡۡۡۡۡۡۡۡۡܡۡۡۡۡۡۡۡۡۡۡۡۡۡ�");

    for (int y = GBOARD_ORIGIN_Y + 1; y < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; y++)
    {
        for (int x = GBOARD_ORIGIN_X + 2; x < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2; x += 2)
        {
            if (x == head->position.X && y == head->position.Y) continue;
            if (stage == 5 && detectCollision(x, y) == 1)
            {
                gotoxycol(x, y, 14, "��");
            }
            if (detectCollision(x, y) != 2)
            {
                if (stage == 5 && detectCollision(x, y) == 1) continue;

                int arrX = (x - GBOARD_ORIGIN_X) / 2;
                int arrY = y - GBOARD_ORIGIN_Y;
                gameBoardInfo[arrY][arrX] = 12;
            }
        }
    }

    setTextColor(15);
}

void removeFever() {
    for (int y = GBOARD_ORIGIN_Y + 1; y < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; y++)
    {
        for (int x = GBOARD_ORIGIN_X + 2; x < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2; x += 2)
        {
            if (x == head->position.X && y == head->position.Y) continue;
            if (detectCollision(x, y) != 2)
            {
                gotoxycol(x, y, 12, "  ");
                if (stage == 5 && detectCollision(x, y) == 1) continue;

                int arrX = (x - GBOARD_ORIGIN_X) / 2;
                int arrY = y - GBOARD_ORIGIN_Y;
                gameBoardInfo[arrY][arrX] = 0;
            }
        }
    }

    for (int i = 0; i < 5; i++) fever[i] = 0;
    gotoxycol(56, 25, 15, "F  E  V  E  R");

    feverStart = 0;

    if (stage == 5) stage5InPlay = 1;

    dragonBallCount = 0;
    humanNum = 0;
    humanHead = NULL;
    addDragonBall();
    createItem();
    showTurnOnArea();
}

void wormhole() {
    COORD out;

    if ((head->position.X == wormholePos[0].X) && (head->position.Y == wormholePos[0].Y))
        out = wormholePos[1];
    else
        out = wormholePos[0];

    Moogi* node = getNode(NULL, head, out);
    head->front = node;
    head = head->front;

    int arrX = (head->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (head->position.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;

    gotoxy(head->position.X, head->position.Y, "��");

    eraseTail();
}

void reset() {
    resetGameBoardInfo();
    direction = 3;
    speed = 15;
    heart = 3;
    dragonBallCount = 0;
    itemCount = 0;
    for (int i = 0; i < 5; i++) fever[i] = 0;
    feverStart = 0;
    feverScore = 0;
    feverWordPos.X = 0;
    feverWordPos.Y = 0;
    head = NULL;
    body = NULL;
    tail = NULL;
    length = 3;
    item = NULL;
    speedScore = 50;
    currentScore = 0;
    moogiColor = 4;
    petGauge = 0;
    petScore = 0;
    pet = NULL;
    stage5InPlay = 0;
    turnOnAndOff = 1;
    humanNum = 0;
    humanAttack = 0;
}

int isIn(int x, int y)
{
    if (y >= GBOARD_ORIGIN_Y + 1 && y < GBOARD_ORIGIN_Y + GBOARD_HEIGHT
        && x >= GBOARD_ORIGIN_X + 2 && x < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2)
        return 1;
    return 0;
}

int nearTheHead(int x, int y)
{
    if (x >= head->position.X - 4 && x <= head->position.X + 4
        && y >= head->position.Y - 2 && y <= head->position.Y + 2)
        return 1;
    return 0;
}

void showTurnOnArea()
{
    if (stage != 5) return;
    if (isFever()) return;

    int yMin = head->position.Y - 2, yMax = head->position.Y + 2;
    int xMin = head->position.X - 4, xMax = head->position.X + 4;

    switch (direction)
    {
    case 0: yMin--; yMax--; break;
    case 1: yMax++; yMax++; break;
    case 2: xMin -= 2; xMax -= 2; break;
    case 3: xMin += 2; xMax += 2; break;
    default:
        break;
    }

    for (int y = yMin; y <= yMax; y++)
    {
        for (int x = xMin; x <= xMax; x += 2)
        {
            if (y <= GBOARD_ORIGIN_Y || y >= GBOARD_ORIGIN_Y + GBOARD_HEIGHT) continue;
            if (x <= GBOARD_ORIGIN_X || x >= GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2) continue;

            int ch = detectCollision(x, y);
            if (!ch) continue;

            COORD pos = { x,y };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

            switch (ch)
            {
            case 1:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); printf("��"); break;
            case 3:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); printf("��"); break;
            case 4:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1); printf("��"); break;
            case 5:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); printf("��"); break;
            case 7:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); printf("F"); break;
            case 8:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); printf("E"); break;
            case 9:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); printf("V"); break;
            case 10:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); printf("E"); break;
            case 11:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); printf("R"); break;
            case 12:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); printf("��"); break;
            default:
                break;
            }
        }
    }

    for (int y = head->position.Y - 6; y <= head->position.Y + 6; y++)
    {
        for (int x = head->position.X - 12; x <= head->position.X + 12; x += 2)
        {
            if (y <= GBOARD_ORIGIN_Y || y >= GBOARD_ORIGIN_Y + GBOARD_HEIGHT) continue;
            if (x <= GBOARD_ORIGIN_X || x >= GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2) continue;
            if (y >= yMin && y <= yMax && x >= xMin && x <= xMax) continue;

            int ch = detectCollision(x, y);
            //if (ch != 4 && ch != 5) {
            COORD pos = { x,y };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
            printf("  ");
            //}
        }
    }
}

void addGameBoardInfo()
{
    for (int y = 1; y < GBOARD_HEIGHT; y++)
    {
        for (int x = 1; x < GBOARD_WIDTH + 2; x++)
        {
            if (mapOfStage5[stage5map][y - 1][x - 1])
            {
                gameBoardInfo[y][x] = 1;
                //gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y, 14, "��");
            }
        }
    }
}

void openingBGM(int play)
{
    openOpeningBgm.lpstrElementName = TEXT("openingBGM.mp3"); //���� ����
    openOpeningBgm.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openOpeningBgm);
    dwID1 = openOpeningBgm.wDeviceID;
    if (play == 0)		//�����ȣ�� ���
        mciSendCommand(dwID1, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&openOpeningBgm); //���� �ݺ� ���
    else	//��� ���� ��ȣ�� ����
        mciSendCommand(dwID1, MCI_PAUSE, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&openOpeningBgm);		//��� ����
}

void playingBGM(int play)
{
    openPlayingBgm.lpstrElementName = TEXT("playingBGM.mp3"); //���� ����
    openPlayingBgm.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openPlayingBgm);
    dwID2 = openPlayingBgm.wDeviceID;
    if (play == 0)		//�����ȣ�� ���
    {
        mciSendCommand(dwID2, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL);
        mciSendCommand(dwID2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&openPlayingBgm); //���� �ݺ� ���
    }
    else	//��� ���� ��ȣ�� ����
        mciSendCommand(dwID2, MCI_PAUSE, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&openPlayingBgm);		//��� ����
}

void selectSound()
{
    openSelectSound.lpstrElementName = TEXT("clickSound.mp3"); //���� ����
    openSelectSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openSelectSound);
    dwID3 = openSelectSound.wDeviceID;
    mciSendCommand(dwID3, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID3, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openSelectSound); //������ �� �� ���
}

void getSound()
{
    openGetSound.lpstrElementName = TEXT("waterdrop.mp3"); //���� ����
    openGetSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openGetSound);
    dwID4 = openGetSound.wDeviceID;
    mciSendCommand(dwID4, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID4, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openGetSound); //������ �� �� ���
}

void collisionSound()
{
    openCollisionSound.lpstrElementName = TEXT("beepbeep.mp3"); //���� ����
    openCollisionSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openCollisionSound);
    dwID5 = openCollisionSound.wDeviceID;
    mciSendCommand(dwID5, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID5, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openCollisionSound); //������ �� �� ���
}

void clearSound()
{
    openClearSound.lpstrElementName = TEXT("clear.mp3"); //���� ����
    openClearSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openClearSound);
    dwID6 = openClearSound.wDeviceID;
    mciSendCommand(dwID6, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID6, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openClearSound); //������ �� �� ���
}

void gameoverSound()
{
    openGameoverSound.lpstrElementName = TEXT("over.mp3"); //���� ����
    openGameoverSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openGameoverSound);
    dwID7 = openGameoverSound.wDeviceID;
    mciSendCommand(dwID7, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID7, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openGameoverSound); //������ �� �� ���
}

void typeSound() {
    openTypeSound.lpstrElementName = TEXT("typeSound.mp3"); //���� ����
    openTypeSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openTypeSound);
    dwID8 = openTypeSound.wDeviceID;
    mciSendCommand(dwID8, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID8, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openTypeSound); //������ �� �� ���
}

void thunderSound() {
    openThunderSound.lpstrElementName = TEXT("thunder.mp3"); //���� ����
    openThunderSound.lpstrDeviceType = TEXT("mpegvideo"); //mp3 ����
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)&openThunderSound);
    dwID9 = openThunderSound.wDeviceID;
    mciSendCommand(dwID9, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
    mciSendCommand(dwID9, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&openThunderSound); //������ �� �� ���
}

void showDragonBallPos()
{
    time_t currentTime;
    time(&currentTime);

    for (int i = 0; i < stage5DragonBallNum; i++)
    {
        if ((int)difftime(currentTime, stage5ShowDragonBall[i]) <= 2)
        {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), stage5DragonBallPos[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); printf("��");
            //showDragonBall(stage5DragonBallPos[i].X, stage5DragonBallPos[i].Y);
        }
        else if (!nearTheHead(stage5DragonBallPos[i].X, stage5DragonBallPos[i].Y))
        {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), stage5DragonBallPos[i]);
            printf("  ");
            //gotoxy(stage5DragonBallPos[i].X, stage5DragonBallPos[i].Y, "  ");

            for (int j = i; j < stage5DragonBallNum - 1; j++)
                stage5DragonBallPos[j] = stage5DragonBallPos[j + 1];
            stage5DragonBallNum--;
        }
    }
}