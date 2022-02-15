//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)//DirectInputのバージョン指定
#include "dinput.h"//入力処理に必要
#include "xaudio2.h"//XAudio２関連のヘッダー
using namespace std;
//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")//描画に必要
#pragma comment(lib,"d3dx9.lib")//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")//DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"dinput8.lib")//入力処理に必要
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xinput.lib")//入力処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1920)//ウィンドウの幅
#define SCREEN_HEIGHT (1080)//ウィンドウの高さ

class CRenderer;
//プロトタイプ宣言
int GetFps(void);
#endif