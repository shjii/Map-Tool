#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <conio.h>

#define RIGHT 77
#define LEFT 75
#define UP 72
#define DOWN 80
#define ENTER 13
#define WHITEWIN 2
#define BLACKWIN 3
#define DO(t) Beep((DWORD)(131*pow(1.06f,0)),(DWORD)(300*(t)));

void(*GAME)();
enum STATE
{
	GAME_INIT = 0,
	KEY_CHECK,
	GAME_DRAW,
	GAME_CHECK
};

void keyCheck();
void GotoXY(int x, int y);
void Drawboard();
void Judgment();

int g_OmokVoard[19][19];
int g_iCursorX = 19;
int g_iCursorY = 9;
bool g_bPlayer = true;
int g_iGameLoop = 1;
int g_iGameStage = GAME_INIT;
bool g_bRestartGame = false;

void Execute()
{
	GAME();
}