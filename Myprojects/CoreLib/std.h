#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <Windows.h>
using namespace std;

#define GameS int APIENTRY wWinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance, LPWSTR lpCmdLine,int nCmdShow){main demo;
#define GameR if (demo.SetWindow(hInstance)){demo.Run();}return 1;}

#define GameRUN GameS;GameR;