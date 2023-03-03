//=============================================================================
//
// manager.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "tutorial.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "input.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CManager::CManager()
{
	m_pGame = nullptr;
	m_pInput = nullptr;
	m_pRenderer = nullptr;
	m_pFade = nullptr;
	m_nScore = 120;
	m_nScoreOld = 120;
	m_nFadeCnt = 0;
	m_nAirframe = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CManager::~CManager()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	//サウンド
	InitSound(hWnd);

	m_bChange = false;
	m_Mode = TYPE_TITLE;
	m_NextMode = m_Mode;
	CTexture::Create();

	m_pInput = CInput::Create();
	//入力処理の初期化処理
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_pRenderer = new CRenderer;

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pGame = new CTitle;
	if (FAILED(m_pGame->Init()))
	{
		return -1;
	}

	m_pFade = new CFade;
	if (FAILED(m_pFade->Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CManager::Uninit()
{
	//サウンド
	UninitSound();

	//入力処理の終了処理
	m_pInput->Uninit();

	CTexture::Uninit();

	if (m_pGame != nullptr)
	{
		// 終了処理
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pFade != nullptr)
	{
		// 終了処理
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CManager::Update()
{
	m_nFadeCnt--;
	if (m_bChange && m_nFadeCnt < 0)
	{
		m_nFadeCnt = 0;
		m_pFade->FadeOut();
		if (m_pFade->CurrentFadeState())
		{
			m_bChange = false;
			m_Mode = m_NextMode;
			ChangeMode(m_Mode);
		}
	}

	if (m_pInput != nullptr && !m_pFade->CurrentFadeState())
	{
		//入力処理の更新処理
		m_pInput->Update();
	}

	if (m_pGame != nullptr)
	{
		m_pGame->Update();
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CManager::Draw(int nFPS)
{
	m_pRenderer->Draw(nFPS);
}

LPDIRECT3DDEVICE9 CManager::GetDeviceManager()
{
	return m_pRenderer->GetDeviceRenderer();
}

//*****************************************************************************
// モードの変更
//*****************************************************************************
void CManager::ChangeMode(MODE_TYPE type)
{
	if (m_pGame != nullptr)
	{// 終了処理
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		CTexture::Uninit();
	}

	switch (type)
	{
	case TYPE_TITLE:
		m_pGame = new CTitle;
		break;
	case TYPE_TUTORIAL:
		m_pGame = new CTutorial;
		break;
	case TYPE_GAME:
		m_pGame = new CGame;
		break;
	case TYPE_RESULT:
		m_pGame = new CResult;
		break;
	case TYPE_MAX:
		break;
	default:
		break;
	}

	CTexture::Create();
	if (FAILED(m_pGame->Init()))
	{
		return;
	}
}

//*****************************************************************************
// 次のモードの設定
//*****************************************************************************
void CManager::NextMode(MODE_TYPE type,int nFadeCnt)
{
	m_nFadeCnt = nFadeCnt;
	m_bChange = true;
	m_NextMode = type;
}

//*****************************************************************************
// スコアの更新
//*****************************************************************************
void CManager::SetScore(int nScore)
{
	m_nScore = nScore;
}
