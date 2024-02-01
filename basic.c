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
Moogi* moogiTail = NULL;    //이거두개는 안쓰는듯???
pItem item = NULL;
time_t itemCreationTime;

int length = 3;
int minLength = 2;
int maxLength = 40;
int minSpeed = 1;
int maxSpeed = 25;

int currentScore = 0;

Wall* wallHead;
int moogiColor = 4;   //5가지 색상만 사용. 0→4(dark red), 1→6(dark yellow), 2→2(dark green), 3→9(blue), 4→15(white)
int dragonBallColor[5];     //0빨 1노 2초 3파 4흰 고정, 어차피 위치랜덤돌릴거니까 얘 순서는 상관ㄴㄴ
COORD dragonBallPos[5];     //빨노초파흰 순서대로 위치 저장. 알맞은 색깔의 여의주를 먹으면 나머지들은 없애줘야하기때문..하..

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
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "▨▨▨▨▨▨▨▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "▨ GAME  OVER ▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "▨▨▨▨▨▨▨▨");
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
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "▨▨▨▨▨▨▨▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "▨ GAME  OVER ▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "▨▨▨▨▨▨▨▨");
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
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1, "▨▨▨▨▨▨▨▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2, "▨ GAME  OVER ▨");
    gotoxy(GBOARD_WIDTH + GBOARD_ORIGIN_X - 5, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1, "▨▨▨▨▨▨▨▨");
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
        if (y == 0) printf("▨");
        else if (y == GBOARD_HEIGHT) printf("▨");
        else printf("▨");

        setCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("▨");
        else if (y == GBOARD_HEIGHT) printf("▨");
        else printf("▨");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("▨");

        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("▨");
    }
    setTextColor(15);

    switch (stage) {
    case 1:
        gotoxy(INFO_X - 1, STAGE_Y, "[  BASIC VERSION  ]");
        break;
    case 2:
        gotoxy(INFO_X - 1, STAGE_Y, "[  움직이는 벽  ]");
        break;
    case 3:
        gotoxy(INFO_X - 1, STAGE_Y, "[  이무지개  ]");
        break;
    default:
        break;
    }
    gotoxy(INFO_X, HIGHSCORE_Y, "");
    printf("HIGHSCORE : %d 점", stageBestScore[stage - 1]);
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
    gotoxy(head->position.X, head->position.Y, "◎");
    gotoxy(body->position.X, body->position.Y, "●");
    gotoxy(tail->position.X, tail->position.Y, "●");
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

    gotoxy(tail->position.X, tail->position.Y, "●");


    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = tail->position.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 2;
}

void deleteBody() {
    if (length > minLength) { // 최소길이보다 클 때만 실행
        eraseTail();
        length--;
    }
}

void speedUp() {
    speed -= 5;
    if (speed < minSpeed) speed = minSpeed; // 최대 속도 리밋
}

void speedDown() {
    speed += 5;
    if (speed > maxSpeed) speed = maxSpeed; // 최소 속도 리밋
}

void createItem() {
    time(&itemCreationTime);

    item->itemNo = rand() % 2 + 4; // 아이템 종류 2가지
    item->pos.X = rand() % GBOARD_WIDTH + GBOARD_ORIGIN_X;
    item->pos.Y = rand() % GBOARD_HEIGHT + GBOARD_ORIGIN_Y;

    do {
        item->pos.X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (item->pos.X % 2 == 1) item->pos.X++;
        item->pos.Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(item->pos.X, item->pos.Y)); // 아무것도 없으면 0 반환됨

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = item->itemNo;

    switch (item->itemNo) {
    case 4:
        setTextColor(1);
        gotoxy(item->pos.X, item->pos.Y, "▲"); break;
    case 5:
        setTextColor(4);
        gotoxy(item->pos.X, item->pos.Y, "▼"); break;
    default:
        gotoxy(INFO_X, ITEM_Y, "아이템 생성에 실패함");
        break;
    }

    setTextColor(15);
}

void deleteItem() { // 삭제만 처리 or 과정까지 처리
    time_t currentTime;
    time(&currentTime);

    if ((int)difftime(currentTime, itemCreationTime) >= 7) {
        item->itemNo = -1;
        printf(" ");
    }
}

/*----------------------------------------------------------------------------------------------*/

void inPlayKeyInput() // 플레이 중의 키 입력
// 이무기 조작 및 일시중지
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

        Sleep(15); // 플레이 속도 조절
    }
}

void shiftUp() // 플레이 중 up 방향키 입력 시
{
    // 충돌 확인, 적절치 못한 방향 전환인지 확인
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    // 방향 업데이트, 이동에 따라 head 및 tail만 redraw
    direction = 0;
    drawHead(nextPos);
    eraseTail();
}

void shiftDown() // 플레이 중 down 방향키 입력 시
{
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 1;
    drawHead(nextPos);
    eraseTail();
}

void shiftLeft() // 플레이 중 left 방향키 입력 시
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

void shiftRight() // 플레이 중 right 방향키 입력 시
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 3;
    drawHead(nextPos);
    eraseTail();
}

void pausePlay() // 일시정지 -> 재시작 혹은 리셋을 기다림
// 화면이 어떻게 구성되는지 몰라서
// 일단 아래쪽 방향키 누르면 홈화면으로 돌아가기
// 위쪽 방향키 누르면 그냥 이어서 게임 진행
// 스페이스 누르면 선택 확정으로 해놨음
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

int detectCollision(int posX, int posY) // 충돌 감지
// 이 함수의 입력 인자로 이무기 머리 위치를 넣어주면 됨
// 충돌 감지하고 싶으면 direction에 따라 한칸 움직인 위치
// 그냥 nextHeadPos() 값을 넣어주면 됨
{
    int arrX = (posX - GBOARD_ORIGIN_X) / 2;
    int arrY = (posY - GBOARD_ORIGIN_Y);

    return gameBoardInfo[arrY][arrX];
}

COORD nextHeadPos() // head의 다음 위치 반환
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

void drawHead(COORD headPos) // 이무기의 이동을 출력(head)
// 이무기 연결리스트 앞에 노드 하나 추가하고
// 머리랑 그 바로 뒤 한 칸(이전 위치에 출력된 머리◎를 지우기 위해)을 출력
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
    gotoxy(head->position.X, head->position.Y, "◎");
    gotoxy(head->back->position.X, head->back->position.Y, "●");

    getSomething();

    int arrX = (headPos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (headPos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;
}

void eraseTail() // 이무기의 이동을 출력(tail)
// 꼬리를 한 칸 앞으로 이동시키고 마지막 노드를 삭제(drawHead에서 하나 추가하니까 length는 똑같음)
// 그리고 지나간 자리에 남은 출력을 공백문자로 지움
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

Moogi* getNode(Moogi* front, Moogi* back, COORD position) // 이무기 노드 하나 생성
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

    drawHead(nextPos);      //얘가 getSomething 부름, getSomething이 countScore랑 getItem 부름,
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
        gotoxy(pos.X, pos.Y, "▨");
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
    printf("SCORE : %d 점", currentScore);
}

void getBestScore() {    //게임 시작 시 플레이 전에 반드시 불러야 한다...(1번만)
    FILE* fp_r = fopen("BestScore.txt", "r");
    if (fp_r == NULL) {
        return;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(fp_r, "%d", &stageBestScore[i]);
    }
    fclose(fp_r);        //파일에서 최고점수 얻어옴 → 파일 비우고 stageBestScore배열에 저장
}

void setBestScore(int score) {            //게임오버됐을때만 부르면 될 듯?
    if (score > stageBestScore[stage - 1]) {
        stageBestScore[stage - 1] = score;
    }
}

void saveBestScore() {    //게임 종료 시 다시 최고점수를 파일에 저장하는 역할
    FILE* fp_w = fopen("BestScore.txt", "w");
    for (int i = 0; i < 5; i++) {
        fprintf(fp_w, "%d\n", stageBestScore[i]);
    }
    fclose(fp_w);
}

int isGameOver() {        //리턴값이 1이나 2면 게임종료해야됨
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2) {
        return 1;        //박으면 1리턴
    }
    return 0;            //아니면 0
}

void getItem() {
    if (detectCollision(head->position.X, head->position.Y) == 4 || detectCollision(head->position.X, head->position.Y) == 5)
    {
        if (item->itemNo == 4) {    //좋은아이템(길이↓, 속도↓, FEVER, 실드, 여의주생성)
            int itemType = (rand() % 2) + 1;//(rand() % 5) + 1; 원래 이건데 지금은 아이템 2개만

            switch (itemType) {
            case 1:        //길이↓
                deleteBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "길이 감소!");
                break;
            case 2:        //속도↓
                speedDown();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "속도 감소!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
        else if (item->itemNo == 5) {    //안좋은아이템 (길이↑, 속도↑)
            int itemType = (rand() % 2) + 1;

            switch (itemType) {
            case 1:        //길이↑
                addBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "길이 증가!");
                break;
            case 2:        //속도↑
                speedUp();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "속도 증가!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
    }
    //0 아무것도없는거, 1 벽장애물, 2 이무기몸, 3 여의주, 4 좋은아이템, 5 나쁜아이템,
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
    gotoxy(x, y, "○");
    setTextColor(15);
}


// 호출 : 구슬을 먹으면 실행
// 이미 벽 또는 아이템이 출력된 곳에 출력하면 X
void addWall() {
    int wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wallX % 2 == 1) wallX++;
        wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(wallX, wallY)); // 아무것도 없으면 0 반환됨

    setTextColor(14);
    gotoxy(wallX, wallY, "▨");
    setTextColor(15);

    int arrX = (wallX - GBOARD_ORIGIN_X) / 2;
    int arrY = (wallY - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 1; // 벽 1

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

void changeMoogiColor() {      //이무기 색깔 세팅, 0빨 1노 2초 3파 4흰
    moogiColor = rand() % 5;
}
void changeMoogiBodyColor() {   //이무기 몸 색깔 바꿈(countScore에서 불림)
    setTextColor_rygbw(moogiColor);
    gotoxy(head->position.X, head->position.Y, "◎");
    Moogi* p = head->back;
    while (p->back != NULL) {
        gotoxy(p->position.X, p->position.Y, "●");
        p = p->back;
    }
}

void setDragonBallColor() {     //한 번만 호출, 초기화용도
    for (int i = 0; i < 5; i++)
        dragonBallColor[i] = i;     //0빨 1노 2초 3파 4흰
}

void setDragonBallPos() {       //한 번만 호출, 초기화용도.
    for (int i = 0; i < 5; i++) {
        dragonBallPos[i].X = 0;
        dragonBallPos[i].Y = 0;
    }
}

void showColorDragonBall(int x, int y, int color) {
    setTextColor_rygbw(color);  //0빨 1노 2초 3파 4흰
    gotoxy(x, y, "○");
    setTextColor(15);
}

void deleteDragonBall() {       //여의주 먹은 후 같은 색 아닌 여의주 지우는 용도(addDragonBall에서 불림)
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