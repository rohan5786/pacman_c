#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
// #include <time.h>

#define ROWS 10
#define COLUMNS 15

int gameover, score;
int px, py;
int gx, gy;
int gtimer;

char maze[ROWS][COLUMNS + 1] = {
    "#######.#######",
    "#..##.....##..#",
    "#.............#",
    "....###.###....",
    "........###....",
    "....###........",
    "....###.###....",
    "#.............#",
    "#..##.....##..#",
    "###############"
};

void setup() {
    gameover = 0;
    score = 0;
    px = 8;
    py = 1;
    gx = 8;
    gy = ROWS - 1;
    gtimer = 0;
}

// clears screen
void clear() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD topLeft = {0, 0};
    DWORD written;

    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(hOut, &screen);

    FillConsoleOutputCharacter(
        hOut, ' ',
        screen.dwSize.X * screen.dwSize.Y,
        topLeft, &written
    );

    SetConsoleCursorPosition(hOut, topLeft);
}

// draws ghost and player
void draw() {
    // puts it at top again
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);

    printf("Controls: WASD | q = quit\n");
    printf("Score: %d\n---------------------------------------------------------------\n\n", score);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (px == j && py == i)
                printf("P");
            else if (gx == j && gy == i)
                printf("G");
            else
                printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n---------------------------------------------------------------\n");
}

// processes player inputs
void checkInput() {
    int npx = px;
    int npy = py;
    if (GetAsyncKeyState('A') & 0x8000) npx--;
    if (GetAsyncKeyState('S') & 0x8000) npy++;
    if (GetAsyncKeyState('D') & 0x8000) npx++;
    if (GetAsyncKeyState('W') & 0x8000) npy--;
    if (GetAsyncKeyState('Q') & 0x8000) gameover = 1;

    if (npx >= 0 && npx < COLUMNS && maze[py][npx] != '#') px = npx;
    if (npy >= 0 && npy < ROWS && maze[npy][px] != '#') py = npy;
}

// score + collision logic
void gameLogic() {
    if (maze[py][px] == '.') {
        score += 10;
        maze[py][px] = ' ';
    }
    // making ghost slower --> runs 5 * 50ms = 250ms period between moves
    gtimer++;
    if (gtimer == 5) {
        int ngx = gx;
        int ngy = gy;

        if (px != gx) ngx = gx + ((px < gx) ? -1 : 1);
        if (py != gy) ngy = gy + ((py < gy) ? -1 : 1);

        if (ngx >= 0 && ngx < COLUMNS && maze[gy][ngx] != '#') gx = ngx;
        if (ngy >= 0 && ngy < ROWS && maze[ngy][gx] != '#') gy = ngy;
        gtimer = 0;
    }

    if (px == gx && py == gy)
        gameover = 1;
}

// running everything
int main() {
    setup();

    // clear cursor stuff idk
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    system("cls");

    while (!gameover) {
        draw();
        checkInput();
        gameLogic();
        Sleep(50);
    }

    clear();
    printf("Game over! Your final score was: %d points.\n", score);

    return 0;
}