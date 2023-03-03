//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "manager.h"
#include "title.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "sound.h"
#include <assert.h>
#include "camera_round.h"
#include "light.h"
#include "ballast_manager.h"
#include "read.h"
#include "mesh_cylinder.h"
#include "motion_parts.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle::CTitle()
{
	m_bPress = false;
	m_Mode = MODE_GAME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CTitle::Init()
{
	//サウンド
	PlaySound(SOUND_LABEL_BGM_TITLE);

	//nullptr
	m_pUi = nullptr;

	//GetUiData
	m_pUi = new CUI;

	//UiInit
	if (FAILED(m_pUi->Init()))
	{
		return -1;
	}

	//GetUiPointer
	C2DPolygon *pPolygon = m_pUi->CreateUi(2);

	//SetUiData
	int nIndex = CTexture::LoadTexture("data/TEXTURE/タイトル.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f));
	pPolygon[0].SetDiagonalLine(600.0f, 600.0f);
	pPolygon[0].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/Pressthekey.png");
	pPolygon[1].SetTextIndex(nIndex);
	pPolygon[1].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f +  200.0f, 0.0f));
	pPolygon[1].SetDiagonalLine(500.0f, 300.0f); 
	pPolygon[1].SetFlashing(INT_MAX);
	pPolygon[1].SetFadeSpeed(0.01f);
	pPolygon[1].SetPolygon();


	m_pGameStart = new C2DPolygon;
	if (FAILED(m_pGameStart->Init()))
	{
		return -1;
	}
	nIndex = CTexture::LoadTexture("data/TEXTURE/ゲームスタート.png");
	m_pGameStart->SetTextIndex(nIndex);
	m_pGameStart->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 400.0f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pGameStart->SetDiagonalLine(600.0f, 600.0f);
	m_pGameStart->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGameStart->SetPolygon();



	m_pTutorial = new C2DPolygon;
	if (FAILED(m_pTutorial->Init()))
	{
		return -1;
	}
	nIndex = CTexture::LoadTexture("data/TEXTURE/チュートリアル.png");
	m_pTutorial->SetTextIndex(nIndex);
	m_pTutorial->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 400.0f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pTutorial->SetDiagonalLine(600.0f, 600.0f);
	m_pTutorial->SetPolygon();

	m_pTextBackground = new C2DPolygon;
	if (FAILED(m_pTextBackground->Init()))
	{
		return -1;
	}
	nIndex = CTexture::LoadTexture("data/TEXTURE/吹き出し.png");
	m_pTextBackground->SetTextIndex(nIndex);
	m_pTextBackground->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 400.0f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pTextBackground->SetDiagonalLine(600.0f, 600.0f);
	m_pTextBackground->SetPolygon();


	//カメラ
	m_pCameraRound = new CCamera_Round;
	if (FAILED(m_pCameraRound->Init()))
	{
		return -1;
	}
	//ライト
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return -1;
	}
	m_LightVec = m_pLight->GetLightVec();

	//瓦礫マネージャーの生成
	m_pBallastManager = new CBallast_Manager;

	//初期化
	if (FAILED(m_pBallastManager->Init()))
	{
		assert(false);
	}

	m_pBallastManager->SetLight(m_LightVec);

	CRead cRead;
	//BG3D
	m_pMeshfieldBG = cRead.ReadMap("data/MAPTXT/map.txt", m_pBallastManager);

	m_pMesh_Cylinder = new CMesh_Cylinder;
	if (FAILED(m_pMesh_Cylinder->Init()))
	{
		return -1;
	}

	Mesh_Cylinder_Structure Mesh_Cylinder_Structure;

	Mesh_Cylinder_Structure.fRadius = 30000.0f;
	Mesh_Cylinder_Structure.fSizeYTop = 100000.0f;
	Mesh_Cylinder_Structure.nPolygonX = 30;
	Mesh_Cylinder_Structure.nPolygonY = 1;
	Mesh_Cylinder_Structure.nTextureNum = 0;
	Mesh_Cylinder_Structure.ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_Cylinder_Structure.ColorMax = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	Mesh_Cylinder_Structure.ColorLowest = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	m_pMesh_Cylinder->SetMesh_Cylinder(Mesh_Cylinder_Structure);

	//入力デバイスの取得
	CInput *pInput = CInput::GetKey();

	//画面内のカーソルを消す
	pInput->SetCursorErase(false);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTitle::Uninit()
{
	//サウンド
	StopSound();

	//UiRelease
	if (m_pUi != nullptr)
	{
		//UiUninit
		m_pUi->Uninit();
		delete m_pUi;
		m_pUi = nullptr;
	}

	if (m_pGameStart != nullptr)
	{
		delete m_pGameStart;
		m_pGameStart = nullptr;
	}

	if (m_pTutorial != nullptr)
	{
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}

	if (m_pTextBackground != nullptr)
	{
		delete m_pTextBackground;
		m_pTextBackground = nullptr;
	}

	//カメラ
	if (m_pCameraRound != nullptr)
	{
		m_pCameraRound->Uninit();
		delete m_pCameraRound;
		m_pCameraRound = nullptr;
	}

	//ライト
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//瓦礫マネージャー
	if (m_pBallastManager != nullptr)
	{
		m_pBallastManager->Uninit();
		delete m_pBallastManager;
		m_pBallastManager = nullptr;
	}

	//メッシュフィールド
	if (m_pMeshfieldBG != nullptr)
	{
		m_pMeshfieldBG->Uninit();
		delete m_pMeshfieldBG;
		m_pMeshfieldBG = nullptr;
	}

	if (m_pMesh_Cylinder != nullptr)
	{
		m_pMesh_Cylinder->Uninit();
		delete m_pMesh_Cylinder;
		m_pMesh_Cylinder = nullptr;
	}

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CTitle::Update()
{
	//GetInput
	CInput *pInput = CInput::GetKey();

	//UiUpdate
	m_pUi->Update();

	
	if (pInput->Trigger(KEY_RIGHT) && m_bPress)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_LEFT_RIGHT);
		m_pGameStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTutorial->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pGameStart->SetFlashing(0);
		m_pTextBackground->SetPos(m_pTutorial->GetPos());
		m_Mode = MODE_TUTORIAL;
	}
	else if (pInput->Trigger(KEY_LEFT) && m_bPress)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_LEFT_RIGHT);
		m_pGameStart->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pTutorial->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTutorial->SetFlashing(0);
		m_pTextBackground->SetPos(m_pGameStart->GetPos());
		m_Mode = MODE_GAME;
	}

	switch (m_Mode)
	{
	case CTitle::MODE_GAME:
		m_pGameStart->SetFlashing(30);
		m_pGameStart->SetFadeSpeed(0.01f);
		break;
	case CTitle::MODE_TUTORIAL:
		m_pTutorial->SetFlashing(30);
		m_pTutorial->SetFadeSpeed(0.01f);
		break;
	case CTitle::MODE_MAX:
	default:
		assert(false);
		break;
	}

	m_pGameStart->Update();
	m_pTutorial->Update();
	m_pTextBackground->Update();

	m_pCameraRound->Update();

	m_pBallastManager->Update();

	CMotionParts::ALLUpdate();

	//EndTitle
	if (pInput->Trigger(KEY_DECISION) && m_bPress)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_DETERMINATION);
		CManager * pManager = GetManager();
		switch (m_Mode)
		{
		case CTitle::MODE_GAME:
			pManager->NextMode(TYPE_GAME);
			break;
		case CTitle::MODE_TUTORIAL:
			pManager->NextMode(TYPE_TUTORIAL);
			break;
		case CTitle::MODE_MAX:
		default:
			assert(false);
			break;
		}
	}
	else if (pInput->Trigger(KEY_DECISION) && !m_bPress)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_DETERMINATION);
		m_bPress = true;
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTitle::Draw()
{
	//カメラ
	m_pCameraRound->SetCamera();

	m_pMesh_Cylinder->Draw();

	m_pMeshfieldBG->Draw();

	CMotionParts::ALLDraw();

	m_pBallastManager->Draw();


	if (!m_bPress)
	{
		//UiDraw
		m_pUi->Draw();
	}
	else
	{
		m_pTextBackground->Draw();
		m_pGameStart->Draw();
		m_pTutorial->Draw();
	}

}