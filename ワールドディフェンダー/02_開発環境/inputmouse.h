//=============================================================================
//
// 入力処理 [directmouse.h]
// Author1 : 唐﨑結斗
// Author2 : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _INPUTMOUSE_H_		//このマクロ定義がされなかったら
#define _INPUTMOUSE_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include "DirectInput.h"

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputMouse : public CDirectInput
{
public:
	

private:
	static const int MAX_MOUSE_KEY = 8;		//マウスの使ってないキーを含めた最大数

public:
	CInputMouse();
	~CInputMouse()override;
	HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;		//マウスの初期化
	void Uninit(void)override;									//マウスの終了処理
	void Update(void)override;									//マウスの更新処理

	bool GetPress(MOUSE_KEY eKey);				//プレス処理
	bool GetTrigger(MOUSE_KEY eKey);			//トリガー処理
	bool GetRelease(MOUSE_KEY eKey);			//リリース情報の取得
	bool GetPressAll();							//オールプレス処理	
	bool GetTriggerAll();						//オールトリガー処理
	bool GetReleaseAll() ;						//オールリリース情報の取得
	D3DXVECTOR3 GetMouseCursor(void);		// マウスカーソルのスクリーン座標の取得
	int GetMouseWheel(void);				// マウスのホイールの動き処理
	D3DXVECTOR3 GetMouseMove(void);			// マウスの移動量を出力処理
	void SetCursorErase(bool bUse) { m_bCursorErase = bUse; } //画面内の時カーソルを消すかどうか

private:
	LPDIRECTINPUTDEVICE8 m_pDevMouse;				//入力でパスへのポインタ
	DIMOUSESTATE2 m_aKeyState;						//マウスのプレス処理
	DIMOUSESTATE2 m_aKeyStateTrigger;				//マウスのトリガー処理
	DIMOUSESTATE2 m_aKeyStateRelease;				//マウスのトリガー処理
	POINT m_Pos;									//マウスのカーソル用
	HWND m_hWnd;									//ウィンドウハンドル
	bool m_bCursorErase;							//カーソルを消すかどうか
	float m_fWidthWnd;								//ウィンドウの横幅
	float m_fHeightWnd;								//ウィンドウの高さ

};
#endif