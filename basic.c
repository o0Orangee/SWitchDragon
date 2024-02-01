#pragma warning(disable:4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32

// size of gameboard
#define GBOARD_WIDTH 26
#define GBOARD_HEIGHT 18

// starting point of gameboard
#define GBOARD_ORIGIN_X 14
#define GBOARD_ORIGIN_Y 5

// size of feverboard
#define FBOARD_WIDTH 7
#define FBOARD_HEIGHT 2

// starting point of feverboard
#define FBOARD_ORIGIN_X 70
#define FBOARD_ORIGIN_Y 29

#define INFO_X 77

#define STAGE_Y 5
#define HIGHSCORE_Y 7
#define SCORE_Y 9
#define PET_Y 11
#define FEVER_Y 13
#define ITEM_Y 15
#define PAUSE_Y 17
#define OVER_Y 19


typedef struct Moogi
{
    struct Moogi* front;
    struct Moogi* back;
    COORD position;
    char direct;
}Moogi;

typedef struct _Item {
    COORD pos;
    int itemNo;
} Item, * pItem;

typedef struct Wall {
    COORD position;
    struct Wall* next;
}Wall;

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int direction = 3;
int stage = 1;
int stageBestScore[5] = { 0,0,0,0,0 };
int speed = 15;
int shield = 0;
int fever[5] = { 0,0,0,0,0 };
Moogi* head = NULL;
Moogi* body = NULL;
Moogi* tail = NULL;

Moogi* moogiHead = NULL;
Moogi* moogiTail = NULL;    //�̰ŵΰ��� �Ⱦ��µ�???
pItem item = NULL;
time_t itemCreationTime;

int length = 3;
int minLength = 2;
int maxLength = 40;
int minSpeed = 1;
int maxSpeed = 25;

int currentScore = 0;

Wall* wallHead;
int moogiColor = 4;   //5���� ���� ���. 0��4(dark red), 1��6(dark yellow), 2��2(dark green), 3��9(blue), 4��15(white)
int dragonBallColor[5];     //0�� 1�� 2�� 3�� 4�� ����, ������ ��ġ���������Ŵϱ� �� ������ �������
COORD dragonBallPos[5];     //���������� ������� ��ġ ����. �˸��� ������ �����ָ� ������ ���������� ��������ϱ⶧��..��..

void stage1();
void stage2();
void stage3();

void console();
void setCurrentCursorPos(int x, int y);
COORD getCurrentCursorPos();
void removeCursor();
void gotoxy(int x, int y, char* ch);
void setTextColor(int colorNum);

void initGameBoardInfo();
void drawBoard();
void title();

void initMoogi();
void addBody();
void deleteBody();
void speedUp();
void speedDown();
void createItem();
void deleteItem();

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
void wallMove();

void countScore();
void getBestScore();
void setBestScore(int score);
void saveBestScore();
int isGameOver();
void getItem();
void addDragonBall();
void showDragonBall(int arrX, int arrY);
void addWall();

void setTextColor_rygbw(int colorNum);
void changeMoogiColor();
void changeMoogiBodyColor();
void setDragonBallColor();
void setDragonBallPos();
void showColorDragonBall(int x, int y, int color);
void deleteDragonBall();

void printMatrix();

int main()
{
    console();
    removeCursor();
    title();
    

    switch (stage)
    {
    case 1:
        stage1();
        break;
    case 2:
        stage2();
        break;
    case 3:
        stage3();
        break;
    default:
        break;
    }

    return 0;
}

void stage1()
{
    srand((unsigned int)time(NULL));

    getBestScore();
    drawBoard();
    initMoogi();

    addDragonBall();
    createItem();

    while (1)
    {
        if (isGameOver()) break;

        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();

        }
    }

    setBestScore(currentScore);

    setTextColor(4);
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "�ɢɢɢɢɢɢɢ�");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "�� GAME  OVER ��");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "�ɢɢɢɢɢɢɢ�");
    setTextColor(15);
    getchar();
}

void stage2()
{
    srand((unsigned int)time(NULL));

    getBestScore();
    drawBoard();
    initMoogi();

    addDragonBall();
    createItem();

    wallHead = (Wall*)malloc(sizeof(Wall));
    wallHead->next = NULL;

    while (1)
    {
        if (isGameOver()) break;

        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();

        }
    }

    setBestScore(currentScore);

    setTextColor(4);
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "�ɢɢɢɢɢɢɢ�");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "�� GAME  OVER ��");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "�ɢɢɢɢɢɢɢ�");
    setTextColor(15);
    getchar();
}

void stage3()
{
    srand((unsigned int)time(NULL));
    setDragonBallColor();
    setDragonBallPos();

    getBestScore();
    drawBoard();
    initMoogi();

    createItem();

    addDragonBall();

    while (1)
    {
        if (isGameOver()) break;

        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();
            
            /*gotoxy(0, 2, "");
            printf("%d", moogiColor);

            gotoxy(GBOARD_WIDTH + 3, GBOARD_HEIGHT + 5, "");
            printMatrix();*/
        }
    }

    setBestScore(currentScore);

    setTextColor(4);
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "�ɢɢɢɢɢɢɢ�");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "�� GAME  OVER ��");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "�ɢɢɢɢɢɢɢ�");
    setTextColor(15);
    getchar();
}

void printMatrix()
{
    for (int i = 0; i < GBOARD_HEIGHT + 1; i++)
    {
        setCurrentCursorPos(INFO_X, PAUSE_Y + 2 + i);
        for (int j = 0; j < GBOARD_WIDTH + 2; j++)
        {
            if (!gameBoardInfo[i][j]) printf("  ");
            else printf("%d ", gameBoardInfo[i][j]);
        }
    }
}

void console() { // set console size
    system("mode con:cols=120 lines=33");
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
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
// 0(Black) 1(Dark Blue) 2(Dark Green) 3(Dark Sky Blue) 4(Dark Red)
// 5(Dark Purple) 6(Dark Yellow) 7(Gray) 8(Dark Gray) 9(Blue)
// 10(Green) 11(Sky Blue) 12(Red) 13(Purple) 14(Yellow) 15(White)


/*-----------------------------------------------------------------------------------------*/

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

void drawBoard() {
    initGameBoardInfo();
    setTextColor(14);
    for (int y = 0; y <= GBOARD_HEIGHT; y++) {
        setCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("��");
        else if (y == GBOARD_HEIGHT) printf("��");
        else printf("��");

        setCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("��");
        else if (y == GBOARD_HEIGHT) printf("��");
        else printf("��");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("��");

        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("��");
    }
    setTextColor(15);

    switch (stage) {
    case 1:
        gotoxy(INFO_X - 1, STAGE_Y, "[  BASIC VERSION  ]");
        break;
    case 2:
        gotoxy(INFO_X - 1, STAGE_Y, "[  �����̴� ��  ]");
        break;
    case 3:
        gotoxy(INFO_X - 1, STAGE_Y, "[  �̹�����  ]");
        break;
    default:
        break;
    }
    gotoxy(INFO_X, HIGHSCORE_Y, "");
    printf("HIGHSCORE : %d ��", stageBestScore[stage - 1]);
}

void title() {
    removeCursor();
    gotoxy(40, 12, "Press '1' to start the stage1 !!");
    gotoxy(40, 13, "Press '2' to start the stage2 !!");
    gotoxy(40, 14, "Press '3' to start the stage3 !!");

    int key;

    while (1) {
        if (_kbhit())
        {
            key = _getch();
            if (key == '1')
            {
                stage = 1;
                break;
            }
            if (key == '2')
            {
                stage = 2;
                break;
            }
            if (key == '3')
            {
                stage = 3;
                break;
            }
        }
    }

    system("cls");
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

    gotoxy(tail->position.X, tail->position.Y, "��");


    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = tail->position.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 2;
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
    default:
        gotoxy(INFO_X, ITEM_Y, "������ ������ ������");
        break;
    }

    setTextColor(15);
}

void deleteItem() { // ������ ó�� or �������� ó��
    time_t currentTime;
    time(&currentTime);

    if ((int)difftime(currentTime, itemCreationTime) >= 7) {
        item->itemNo = -1;
        printf(" ");
    }
}

/*----------------------------------------------------------------------------------------------*/

void inPlayKeyInput() // �÷��� ���� Ű �Է�
// �̹��� ���� �� �Ͻ�����
{
    int i, key; // , isDone = 0;

    for (i = 0; i < speed; i++)
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
}

void shiftLeft() // �÷��� �� left ����Ű �Է� ��
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    if (direction == 2 || direction == 3) return;

    direction = 2;
    drawHead(nextPos);
    eraseTail();
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
}

void pausePlay() // �Ͻ����� -> ����� Ȥ�� ������ ��ٸ�
// ȭ���� ��� �����Ǵ��� ����
// �ϴ� �Ʒ��� ����Ű ������ Ȩȭ������ ���ư���
// ���� ����Ű ������ �׳� �̾ ���� ����
// �����̽� ������ ���� Ȯ������ �س���
{

    gotoxy(INFO_X, PAUSE_Y, "PRESS SPACE TO PLAY!!");
    while (1)
    {
        if (_kbhit() != 0)
        {
            int key = _getch();

            if (key == SPACE) break;
        }
        Sleep(speed);
    }
    gotoxy(INFO_X, PAUSE_Y, "                           ");
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
    gotoxy(head->position.X, head->position.Y, "��");
    gotoxy(head->back->position.X, head->back->position.Y, "��");

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

    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 0;

    gotoxy(tail->position.X, tail->position.Y, "  ");

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
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2)
        return 0;

    drawHead(nextPos);      //�갡 getSomething �θ�, getSomething�� countScore�� getItem �θ�,
    eraseTail();
    if (stage == 2) wallMove();

    return 1;
}

void getSomething()
{
    countScore();
    getItem();
}

void wallMove()
{
    Wall* p = wallHead;

    while (p->next != NULL)
    {
        p = p->next;

        COORD pos = { p->position.X, p->position.Y };

        while (1)
        {
            pos.X = p->position.X;
            pos.Y = p->position.Y;

            int move = rand() % 4;
            switch (move)
            {
            case 0:
                pos.Y--;
                break;
            case 1:
                pos.Y++;
                break;
            case 2:
                pos.X -= 2;
                break;
            case 3:
                pos.X += 2;
                break;
            default:
                break;
            }

            if (!detectCollision(pos.X, pos.Y)) break;
        }

        int arrX = (p->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (p->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;

        arrX = (pos.X - GBOARD_ORIGIN_X) / 2;
        arrY = (pos.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 1;

        setTextColor(14);
        gotoxy(p->position.X, p->position.Y, "  ");
        gotoxy(pos.X, pos.Y, "��");
        setTextColor(15);

        p->position.X = pos.X;
        p->position.Y = pos.Y;

        //p = p->next;
    }
}

/*-------------------------------------------------------------------------------------------*/

void countScore() {
    if (detectCollision(head->position.X, head->position.Y) == 3) {
        currentScore += 10;
        addBody();
        if (stage == 2) addWall();
        if (stage == 3) {
            changeMoogiColor();
            changeMoogiBodyColor();
        }
        addDragonBall();
    }

    gotoxy(INFO_X, SCORE_Y, "");
    setTextColor(15);
    printf("SCORE : %d ��", currentScore);
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
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2) {
        return 1;        //������ 1����
    }
    return 0;            //�ƴϸ� 0
}

void getItem() {
    if (detectCollision(head->position.X, head->position.Y) == 4 || detectCollision(head->position.X, head->position.Y) == 5)
    {
        if (item->itemNo == 4) {    //����������(���̡�, �ӵ���, FEVER, �ǵ�, �����ֻ���)
            int itemType = (rand() % 2) + 1;//(rand() % 5) + 1; ���� �̰ǵ� ������ ������ 2����

            switch (itemType) {
            case 1:        //���̡�
                deleteBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "���� ����!");
                break;
            case 2:        //�ӵ���
                speedDown();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "�ӵ� ����!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
        else if (item->itemNo == 5) {    //������������ (���̡�, �ӵ���)
            int itemType = (rand() % 2) + 1;

            switch (itemType) {
            case 1:        //���̡�
                addBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "���� ����!");
                break;
            case 2:        //�ӵ���
                speedUp();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "�ӵ� ����!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
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
    }
    else {
        do {
            ballX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (ballX % 2 == 1) ballX++;
            ballY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
        } while (detectCollision(ballX, ballY));

        int arrX = (ballX - GBOARD_ORIGIN_X) / 2;
        int arrY = (ballY - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 3;
        showDragonBall(ballX, ballY);
    }
}

void showDragonBall(int x, int y) {
    setTextColor(6);
    gotoxy(x, y, "��");
    setTextColor(15);
}


// ȣ�� : ������ ������ ����
// �̹� �� �Ǵ� �������� ��µ� ���� ����ϸ� X
void addWall() {
    int wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wallX % 2 == 1) wallX++;
        wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(wallX, wallY)); // �ƹ��͵� ������ 0 ��ȯ��

    setTextColor(14);
    gotoxy(wallX, wallY, "��");
    setTextColor(15);

    int arrX = (wallX - GBOARD_ORIGIN_X) / 2;
    int arrY = (wallY - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 1; // �� 1

    Wall* node = (Wall*)malloc(sizeof(Wall));
    node->position.X = wallX;
    node->position.Y = wallY;
    node->next = NULL;

    Wall* p = wallHead;
    while (p->next != NULL) p = p->next;
    p->next = node;
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
            int arrX = (dragonBallPos[i].X- GBOARD_ORIGIN_X) / 2;
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