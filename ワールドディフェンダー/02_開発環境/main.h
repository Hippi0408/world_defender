//=============================================================================
//
// main.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MAIN_H_			//このマクロ定義がされなかったら
#define _MAIN_H_			//2重インクルード防止のマクロ定義


//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
	// ウインドウのクラス名
	LPCTSTR CLASS_NAME = _T("AppClass");
	// ウインドウのキャプション名
	LPCTSTR WINDOW_NAME = _T("WorldDefencer");
}

// スクリーンの幅
const int SCREEN_WIDTH = 1600;
// スクリーンの高さ
const int SCREEN_HEIGHT = 900;

// スクリーンの幅
const int SCREEN_GAME_WIDTH = 720;
// スクリーンの高さ
const int SCREEN_GAME_HEIGHT = 840;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点情報[ライン]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DCOLOR col;			//頂点カラー
}VERTEX_LINE;


// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;		//テクスチャ座標
};

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
}VERTEX_3D;

class CRenderer;
class CManager;

CManager *GetManager();


#endif