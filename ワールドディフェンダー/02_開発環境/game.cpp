//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "game.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "player.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "billboard.h"
#include "motion_parts.h"
#include "tps_camera.h"
#include "read.h"
#include "ballast_manager.h"
#include <assert.h>
#include "mesh_cylinder.h"
#include "sound.h"
#include "gauge.h"
#include "camera_round.h"
#include "time.h"
#include "adrenaline_item.h"
#include "ufo.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame::CGame()
{
	m_bGameEnd = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGame::~CGame()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CGame::Init()
{

	//�T�E���h��~
	StopSound();
	//�T�E���h
	PlaySound(SOUND_LABEL_BGM_GAME);

	//�J����
	m_pCamera = new CTpsCamera;
	if (FAILED(m_pCamera->Init()))
	{
		return -1;
	}
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

	
	

	m_pPlayer = new CPlayer;
	m_pPlayer->SetLight(m_LightVec);
	if (FAILED(m_pPlayer->Init()))
	{
		return -1;
	}
	

	m_pEnmeyManager = new CEnemy_Manager;
	m_pEnmeyManager->SetLight(m_LightVec);
	if (FAILED(m_pEnmeyManager->Init()))
	{
		return -1;
	}
	

	EnemyInitData EnemyInitData;

	EnemyInitData.fmove = 10.0f;
	EnemyInitData.pos = D3DXVECTOR3(0.0f,0.0f, 9000.0f);
	EnemyInitData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyInitData.type = ENEMY_02;

	m_pEnmeyManager->CreateEnemy(EnemyInitData);

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
	m_pMeshfieldBG = cRead.ReadMap("data/MAPTXT/map.txt",m_pBallastManager);

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
	Mesh_Cylinder_Structure.ParentPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	Mesh_Cylinder_Structure.ColorMax = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	Mesh_Cylinder_Structure.ColorLowest = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	m_pMesh_Cylinder->SetMesh_Cylinder(Mesh_Cylinder_Structure);

	m_pTime = new CTime;
	if (FAILED(m_pTime->Init(TIME_MAX)))
	{
		return -1;
	}

	//�A�C�e��������
	if (FAILED(CAdrenalineItem::AllInit()))
	{
		return -1;
	}

	CAdrenalineItem::AllSetLightVec(m_LightVec);

	//UFO�̐���
	m_pUfo = new CUfo;

	//UFO�̏�����
	if (FAILED(m_pUfo->Init()))
	{
		return -1;
	}

	//�e�̐ݒ�
	m_pUfo->SetLightVec(m_LightVec);

	//�ׂ����ݒ�
	m_pUfo->Set3DObject(cRead.ReadXFile("data/MODEL/UFO.x"), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�����G�l�~�[����
	m_pEnmeyManager->InitPopEnemy();

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CGame::Uninit()
{
	//�T�E���h
	StopSound();

	//�J����
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
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

	//Player
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	if (m_pEnmeyManager != nullptr)
	{
		m_pEnmeyManager->Uninit();
		delete m_pEnmeyManager;
		m_pEnmeyManager = nullptr;
	}

	if (m_pMesh_Cylinder != nullptr)
	{
		m_pMesh_Cylinder->Uninit();
		delete m_pMesh_Cylinder;
		m_pMesh_Cylinder = nullptr;
	}

	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	if (m_pUfo != nullptr)
	{
		m_pUfo->Uninit();
		delete m_pUfo;
		m_pUfo = nullptr;
	}

	CAdrenalineItem::AllUninit();

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

	CGauge::AllUninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CGame::Update()
{

	m_pMesh_Cylinder->Update(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�J�����̍X�V
	if(m_bRoundCamera)
	{
		m_pCameraRound->Update();
	}
	else
	{
		m_pCamera->Update();
	}

	//����J�����p�̃J�E���g�`�F�b�N
	if (m_nRoundCnt > m_nRoundCntMax)
	{
		m_bRoundCamera = false;
		m_pTime->SetTimeUpdate(true);
	}
	else
	{
		m_nRoundCnt++;
	}

	m_pEnmeyManager->Update();

	//�Q�[���G���h�ł͂Ȃ�������X�V
	if (!m_bGameEnd)
	{
		m_pPlayer->Update();
		m_pTime->Update();
	}

	m_pBallastManager->Update();

	m_pUfo->Update();
	
	CInput *pInput = CInput::GetKey();

	CAdrenalineItem::AllUpdate();

	CMotionParts::ALLUpdate();

	if (m_pTime->CheckTime())
	{
		m_bGameEnd = true;
		CManager * pManager = GetManager();
		pManager->SetScore(999);
		pManager->NextMode(TYPE_RESULT);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CGame::Draw()
{
	//�J�����̍X�V
	if (m_bRoundCamera)
	{
		m_pCameraRound->SetCamera();
	}
	else
	{
		m_pCamera->SetCamera();
	}

	m_pMesh_Cylinder->Draw();

	m_pMeshfieldBG->Draw();

	CMotionParts::ALLDraw();

	m_pBallastManager->Draw();

	m_pEnmeyManager->Draw();

	m_pUfo->Draw();

	CAdrenalineItem::AllDraw();

	if (!m_bRoundCamera)
	{
		//�Q�[�W
		CGauge::AllDraw();

		m_pPlayer->Draw();

		m_pTime->Draw();
	}

}

//*****************************************************************************
// ����J�����̊J�n
//*****************************************************************************
void CGame::SetRoundCamera(int nRoundCntMax)
{
	// ����J�����̊J�n
	m_bRoundCamera = true;

	// ����J�����̍ő�J�E���g��ۑ�
	m_nRoundCntMax = nRoundCntMax;

	// ����J�����̃J�E���g�̏�����
	m_nRoundCnt = 0;
}

//*****************************************************************************
// ����J������PosR�̐ݒ�
//*****************************************************************************
void CGame::SetRoundCameraPosR(D3DXVECTOR3 posR)
{
	m_pCameraRound->SetPosR(posR);
}

//*****************************************************************************
// �J�����̐U���ݒ�
//*****************************************************************************
void CGame::SetVibration(int nVibrationMax,int nVibration)
{
	//�J�����̐U���ݒ�
	if (m_bRoundCamera)
	{
		m_pCameraRound->SetVibration(nVibrationMax, nVibration);
	}
	else
	{
		m_pCamera->SetVibration(nVibrationMax, nVibration);
	}
}

//*****************************************************************************
// �U�������邩�ǂ���
//*****************************************************************************
bool CGame::CheckVibration()
{
	//�J�����̐U���`�F�b�N
	if (m_bRoundCamera)
	{
		return m_pCameraRound->CheckVibration();
	}
	else
	{
		return m_pCamera->CheckVibration();
	}
	return false;
}

//*****************************************************************************
// �^�C���̃n�C�X�R�A�m�F
//*****************************************************************************
void CGame::TimeHighScoreConfirmation()
{
	int nScore = (int)(TIME_MAX - m_pTime->GetTime()) / 60;

	CManager * pManager = GetManager();

	pManager->SetScore(nScore);
}
