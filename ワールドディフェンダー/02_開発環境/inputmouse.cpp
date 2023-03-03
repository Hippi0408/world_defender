//=============================================================================
//
// 入力処理 [directmouse.h]
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : マウス生成を行う
//
//=============================================================================

//***************************************************************************
// インクルード
//***************************************************************************
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "inputmouse.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CInputMouse::CInputMouse()
{
	memset(&m_aKeyState, 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(&m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));

	m_fHeightWnd = 0.0f;
	m_fWidthWnd = 0.0f;
	m_bCursorErase = true;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// 初期化
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : マウスのアクセス権を獲得できた場合、処理を継続する
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{

	// 入力デバイス（マウス）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// ウィンドウハンドルの格納
	m_hWnd = hWnd;

	// Deviceへのアクセス権を獲得
	m_pDevMouse->Acquire();

	//ウィンドウの横幅と高さの取得
	m_fWidthWnd = SCREEN_WIDTH;
	m_fHeightWnd = SCREEN_HEIGHT;

	return S_OK;
}

//=============================================================================
// 終了
// Author : KOZUNA HIROHITO
// 概要 : マウスのアクセス権を解放し、デバイスを解放する
//=============================================================================
void CInputMouse::Uninit(void)
{
	//入力デバイス（キーボード）の放棄
	if (m_pDevMouse != nullptr)
	{
		m_pDevMouse->Unacquire();		//キーボードへのアクセス権を放棄
		m_pDevMouse->Release();
		m_pDevMouse = nullptr;
	}
}

//=============================================================================
// 更新
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : マウス入力された情報を保存する
//=============================================================================
void CInputMouse::Update(void)
{
	// 変数宣言
	DIMOUSESTATE2 aKeyState;		// マウス入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MAX_MOUSE_KEY; nCntKey++)
		{
			// マウストリガー情報を保存
			m_aKeyStateTrigger.rgbButtons[nCntKey] = ~m_aKeyState.rgbButtons[nCntKey] & aKeyState.rgbButtons[nCntKey];

			// マウスリリース情報を保存
			m_aKeyStateRelease.rgbButtons[nCntKey] = m_aKeyState.rgbButtons[nCntKey] & ~aKeyState.rgbButtons[nCntKey];

			// マウスプレス情報を保存
			m_aKeyState = aKeyState;
		}
	}
	else
	{
		// マウスへのアクセス権を獲得
		m_pDevMouse->Acquire();
	}

	//カーソルをオンオフ
	ShowCursor(m_bCursorErase);
}

//=============================================================================
//  マウスプレス情報の取得
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetPress(MOUSE_KEY eKey)
{
	return (m_aKeyState.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// マウストリガー情報の取得
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetTrigger(MOUSE_KEY eKey)
{
	return (m_aKeyStateTrigger.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// マウスリリース情報の取得
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetRelease(MOUSE_KEY eKey)
{
	return (m_aKeyStateRelease.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// 全キープレスの情報の取得
// Author : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetPressAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetPress((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// 全キープレスの情報の取得
// Author : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetTriggerAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetTrigger((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// 全キープレスの情報の取得
// Author : KOZUNA HIROHITO
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CInputMouse::GetReleaseAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetRelease((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// マウスカーソルの位置の取得
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
// 概要 : マウスカーソルの位置を取得し、数値を返す
//=============================================================================
D3DXVECTOR3 CInputMouse::GetMouseCursor(void)
{
	// スクリーン座標上のマウスカーソルの位置の取得
	GetCursorPos(&m_Pos);

	// ウィンドウ上のマウスカーソルの位置の取得
	ScreenToClient(m_hWnd, &m_Pos);

	return D3DXVECTOR3((float)m_Pos.x, (float)m_Pos.y, 0.0f);
}

//=============================================================================
// マウスホイールの移動量取得
// Author : 唐﨑結斗
// 概要 : マウスホイールの移動量の数値を返す
//=============================================================================
int CInputMouse::GetMouseWheel(void)
{
	return (int)m_aKeyState.lZ;
}

//=============================================================================
// マウスの移動量取得
// Author : 唐﨑結斗
// 概要 : マウスの移動量の数値を返す
//=============================================================================
D3DXVECTOR3 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR3((float)(m_aKeyState.lX), (float)(m_aKeyState.lY), (float)(m_aKeyState.lZ));
}