//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "result.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include <assert.h>
#include "camera_round.h"
#include "light.h"
#include "ballast_manager.h"
#include "read.h"
#include "mesh_cylinder.h"
#include "number.h"
#include "2dpolygon.h"

const float CResult::SCORE_SIZE = 250.0f;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CResult::CResult()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CResult::~CResult()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CResult::Init()
{
	//�T�E���h
	PlaySound(SOUND_LABEL_BGM_RESULT);

	
	//�J����
	m_pCameraRound = new CCamera_Round;
	if (FAILED(m_pCameraRound->Init()))
	{
		return -1;
	}
	//���C�g
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return -1;
	}
	m_LightVec = m_pLight->GetLightVec();

	//���I�}�l�[�W���[�̐���
	m_pBallastManager = new CBallast_Manager;

	//������
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


	CManager* pManager = GetManager();

	int nNewScore = pManager->GetScore();
	int nOldScore = pManager->GetScoreOld();

	if (nNewScore < nOldScore)
	{
		pManager->SetScoreOld(nNewScore);
	}


	m_pNewScore = new CNumber;

	if (FAILED(m_pNewScore->Init()))
	{
		return -1;
	}

	m_pNewScore->SetNumber(D3DXVECTOR3(700.0f, 700.0f, 0.0f), 3, SCORE_SIZE, nNewScore);

	m_pOldScore = new CNumber;

	if (FAILED(m_pOldScore->Init()))
	{
		return -1;
	}

	m_pOldScore->SetNumber(D3DXVECTOR3(700.0f, 400.0f, 0.0f), 3, SCORE_SIZE, nOldScore);


	m_pOld = new C2DPolygon;

	if (FAILED(m_pOld->Init()))
	{
		return -1;
	}

	int nIndex = CTexture::LoadTexture("data/TEXTURE/���܂ł̋L�^.png");
	m_pOld->SetTextIndex(nIndex);
	m_pOld->SetPos(D3DXVECTOR3(200.0f, 400.0f, 0.0f));
	m_pOld->SetDiagonalLine(200.0f, 200.0f);
	m_pOld->SetPolygon();

	if (nNewScore > 900)
	{
		m_pNew = new C2DPolygon;

		if (FAILED(m_pNew->Init()))
		{
			return -1;
		}

		nIndex = CTexture::LoadTexture("data/TEXTURE/�^�C���I�[�o�[.png");
		m_pNew->SetTextIndex(nIndex);
		m_pNew->SetPos(D3DXVECTOR3(200.0f, 700.0f, 0.0f));
		m_pNew->SetDiagonalLine(200.0f, 200.0f);
		m_pNew->SetPolygon();
	}
	else if (nNewScore < nOldScore)
	{
		m_pNew = new C2DPolygon;

		if (FAILED(m_pNew->Init()))
		{
			return -1;
		}

		nIndex = CTexture::LoadTexture("data/TEXTURE/�L�^�X�V.png");
		m_pNew->SetTextIndex(nIndex);
		m_pNew->SetPos(D3DXVECTOR3(200.0f, 700.0f, 0.0f));
		m_pNew->SetDiagonalLine(200.0f, 200.0f);
		m_pNew->SetFlashing(INT_MAX);
		m_pNew->SetFadeSpeed(0.01f);
		m_pNew->SetPolygon();
	}


	m_pRankingName = new C2DPolygon;

	if (FAILED(m_pRankingName->Init()))
	{
		return -1;
	}

	nIndex = CTexture::LoadTexture("data/TEXTURE/�|���܂łɂ��������b��.png");
	m_pRankingName->SetTextIndex(nIndex);
	m_pRankingName->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f));
	m_pRankingName->SetDiagonalLine(500.0f, 500.0f);
	m_pRankingName->SetPolygon();



	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CResult::Uninit()
{
	//�T�E���h
	StopSound();

	//�J����
	if (m_pCameraRound != nullptr)
	{
		m_pCameraRound->Uninit();
		delete m_pCameraRound;
		m_pCameraRound = nullptr;
	}

	//���C�g
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//���I�}�l�[�W���[
	if (m_pBallastManager != nullptr)
	{
		m_pBallastManager->Uninit();
		delete m_pBallastManager;
		m_pBallastManager = nullptr;
	}

	//���b�V���t�B�[���h
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

	if (m_pNewScore != nullptr)
	{
		m_pNewScore->Uninit();
		delete m_pNewScore;
		m_pNewScore = nullptr;
	}

	if (m_pOldScore != nullptr)
	{
		m_pOldScore->Uninit();
		delete m_pOldScore;
		m_pOldScore = nullptr;
	}

	if (m_pNew != nullptr)
	{
		m_pNew->Uninit();
		delete m_pNew;
		m_pNew = nullptr;
	}

	if (m_pOld != nullptr)
	{
		m_pOld->Uninit();
		delete m_pOld;
		m_pOld = nullptr;
	}

	if (m_pRankingName != nullptr)
	{
		m_pRankingName->Uninit();
		delete m_pRankingName;
		m_pRankingName = nullptr;
	}

	C3DObject::UninitAllModel(); 
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CResult::Update()
{
	m_pCameraRound->Update();

	m_pBallastManager->Update();

	if (m_pOld != nullptr)
	{
		m_pOld->Update();
	}

	if (m_pNew != nullptr)
	{
		m_pNew->Update();
	}

	//GetInput
	CInput *pInput = CInput::GetKey();

	//EndResult
	if (pInput->Trigger(KEY_DECISION))
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_TITLE);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CResult::Draw()
{
	//�J����
	m_pCameraRound->SetCamera();

	m_pMesh_Cylinder->Draw();

	m_pMeshfieldBG->Draw();

	m_pBallastManager->Draw();

	m_pNewScore->Draw();

	m_pOldScore->Draw();

	if (m_pOld != nullptr)
	{
		m_pOld->Draw();
	}

	if (m_pNew != nullptr)
	{
		m_pNew->Draw();
	}

	m_pRankingName->Draw();

}