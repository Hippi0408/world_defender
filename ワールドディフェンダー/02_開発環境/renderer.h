//=============================================================================
//
// renderer.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _RENDERER_H_			//このマクロ定義がされなかったら
#define _RENDERER_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw(const int nFps);
	LPDIRECT3DDEVICE9 GetDeviceRenderer() { return m_pD3DDevice; }

	void WindowChange();

#ifdef _DEBUG
	void DrawFPS(const int nFps);
#endif // _DEBUG
private:
	//ウインドウの状態(true = 非フルスクリーン、false = フルスクリーン)
	bool m_bWindow;
	//ウインドウハンドル
	HWND m_pWindowHandle;
	// Direct3Dオブジェクト
	LPDIRECT3D9 m_pD3D = nullptr;
	// Deviceオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;

#ifdef _DEBUG
	// フォント
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

};
#endif