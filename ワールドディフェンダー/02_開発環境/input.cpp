//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"

//-----------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//このクラスの情報

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//キーボードの情報
	m_pJoyPad = nullptr;		//ジョイパッドの情報
	m_pMouse = nullptr;			//マウスの情報
	m_nOldInputType = INPUT_TYPE_KEYBOARD;
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの生成
	m_pJoyPad = new CInputJoyPad;

	//ジョイパッドの初期化処理
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウスの生成
	m_pMouse = new CInputMouse;

	//マウスの初期化処理
	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInput::Uninit()
{
	//キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//ジョイパッドの破棄
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	//DirectInputオブジェクトの破棄
	CDirectInput::Break();

	//自己破棄
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInput::Update()
{
	//キーボードの更新
	m_pKeyboard->Update();
	//ジョイパッドの更新
	m_pJoyPad->Update();
	//マウスの更新
	m_pMouse->Update();

	//最後に触ったデバイス
	if (m_pJoyPad->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_JOYPAD;
	}
	else if (m_pKeyboard->GetKeyboardAllPress())
	{
		m_nOldInputType = INPUT_TYPE_KEYBOARD;
	}
	else if (m_pMouse->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_MOUSE;
	}
}

//*************************************************************************************
//インプットの生成
//*************************************************************************************
CInput *CInput::Create()
{
	//Inputの自己生成
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//プレス処理(総合)
//*************************************************************************************
bool CInput::Press(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardPress(DIK_W)
			|| m_pKeyboard->GetKeyboardPress(DIK_UP)
			|| m_pJoyPad->GetPressAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_W) && m_pKeyboard->GetKeyboardPress(DIK_A))
			|| (m_pKeyboard->GetKeyboardPress(DIK_UP) && m_pKeyboard->GetKeyboardPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_W) && m_pKeyboard->GetKeyboardPress(DIK_D))
			|| (m_pKeyboard->GetKeyboardPress(DIK_UP) && m_pKeyboard->GetKeyboardPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardPress(DIK_S)
			|| m_pKeyboard->GetKeyboardPress(DIK_DOWN)
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_S) && m_pKeyboard->GetKeyboardPress(DIK_A))
			|| (m_pKeyboard->GetKeyboardPress(DIK_DOWN) && m_pKeyboard->GetKeyboardPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_S) && m_pKeyboard->GetKeyboardPress(DIK_D))
			|| (m_pKeyboard->GetKeyboardPress(DIK_DOWN) && m_pKeyboard->GetKeyboardPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_A)
			|| m_pKeyboard->GetKeyboardPress(DIK_LEFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardPress(DIK_D)
			|| m_pKeyboard->GetKeyboardPress(DIK_RIGHT)
			|| m_pJoyPad->GetPressAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardPress(DIK_RETURN)
			|| m_pJoyPad->GetPressAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardPress(DIK_SPACE)
			|| m_pJoyPad->GetPressAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardPress(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardPress(DIK_B)
			|| m_pJoyPad->GetPressAll(JOYPAD_BACK)
			|| m_pJoyPad->GetPressAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardPress(DIK_LSHIFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_L1))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Press(KEY_UP) || Press(KEY_DOWN) || Press(KEY_LEFT) || Press(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardPress(DIK_P)
			|| m_pJoyPad->GetPressAll(JOYPAD_START))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetKeyboardAllPress()
			|| m_pJoyPad->GetPressAll()
			|| m_pMouse->GetPressAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//トリガー処理(総合)
//*************************************************************************************
bool CInput::Trigger(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_W)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_UP)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_W) && m_pKeyboard->GetKeyboardTrigger(DIK_A))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_UP) && m_pKeyboard->GetKeyboardTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_W) && m_pKeyboard->GetKeyboardTrigger(DIK_D))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_UP) && m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_S)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_DOWN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_S) && m_pKeyboard->GetKeyboardTrigger(DIK_A))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_DOWN) && m_pKeyboard->GetKeyboardTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_S) && m_pKeyboard->GetKeyboardTrigger(DIK_D))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_DOWN) && m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_A)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LEFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_D)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RETURN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_SPACE)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_B)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_BACK)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LSHIFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_P)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_START)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_HOME))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetKeyboardAllTrigger()
			|| m_pJoyPad->GetTriggerAll()
			|| m_pMouse->GetTriggerAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//プレス処理(キーボード)
//*************************************************************************************
bool CInput::Press(int nKey)
{
	return m_pKeyboard->GetKeyboardPress(nKey);
}

//*************************************************************************************
//トリガー処理(キーボード)
//*************************************************************************************
bool CInput::Trigger(int nkey)
{
	return m_pKeyboard->GetKeyboardTrigger(nkey);
}

//*************************************************************************************
//リリース処理(キーボード)
//*************************************************************************************
bool CInput::Release(int nkey)
{
	return m_pKeyboard->GetKeyboardRelease(nkey);
}

//*************************************************************************************
//プレス処理(ジョイパッド)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//トリガー処理(ジョイパッド)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//プレス処理(マウス)
//*************************************************************************************
bool CInput::Press(MOUSE_KEY Key)
{
	return m_pMouse->GetPress(Key);
}

//*************************************************************************************
//トリガー処理(マウス)
//*************************************************************************************
bool CInput::Trigger(MOUSE_KEY Key)
{
	return m_pMouse->GetTrigger(Key);
}

//*************************************************************************************
//リリース処理(マウス)
//*************************************************************************************
bool CInput::Release(MOUSE_KEY Key)
{
	return m_pMouse->GetRelease(Key);
}

//*************************************************************************************
//十字キーのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey(int nNum)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	if (m_pJoyPad->GetJoyPadNumMax() <= nNum)
	{
		return VectorMove;
	}

	int nCheck = m_pJoyPad->GetCross(nNum);

	if (nCheck > 360 || nCheck < 0)
	{//十字キーの角度が指定外だったら０を返す
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

D3DXVECTOR3 CInput::VectorMoveKeyAll()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nJoyPad = m_pJoyPad->GetJoyPadNumMax();
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	for (int nCnt = 0; nCnt < nJoyPad; nCnt++)
	{
		int nCheck = m_pJoyPad->GetCross(nCnt);

		if (nCheck > 360 || nCheck < 0)
		{//十字キーの角度が指定外だったら０を返す
			return VectorMove;
		}

		float fRot = D3DXToRadian(nCheck);

		VectorMove.x = sinf(fRot);
		VectorMove.y = -cosf(fRot);

		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);

		if (VectorMove != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			return VectorMove;
		}

	}
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStickAll(bool bleftandright)
{
	D3DXVECTOR3 VectorNull = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		VectorMove = m_pJoyPad->GetJoyStickData(nCnt, bleftandright);
		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);
		if (VectorNull != VectorMove)
		{
			return  VectorMove;
		}
	}

	return VectorNull;
}

//*************************************************************************************
// マウスカーソルのスクリーン座標の取得
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseCursor(void)
{
	return m_pMouse->GetMouseCursor();
}

//*************************************************************************************
// マウスのホイールの動き処理
//*************************************************************************************
int CInput::GetMouseWheel(void)
{
	return m_pMouse->GetMouseWheel();
}

//*************************************************************************************
// マウスの移動量を出力処理
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseMove(void)
{
	return m_pMouse->GetMouseMove();
}

//*************************************************************************************
// 画面内のカーソルを消すかどうか
//*************************************************************************************
void CInput::SetCursorErase(bool bUse)
{
	// 画面内のカーソルを消すかどうか
	m_pMouse->SetCursorErase(bUse);
}
