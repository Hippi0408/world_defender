//=============================================================================
//
// tutorial.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "tutorial.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "player.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "motion_parts.h"
#include "tps_camera.h"
#include "read.h"
#include "ballast_manager.h"
#include <assert.h>
#include "mesh_cylinder.h"
#include "sound.h"
#include "gauge.h"
#include "adrenaline_item_tutorial.h"
#include "adrenaline_item.h"
#include "2dpolygon.h"
#include "enemy_tutorial.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorial::CTutorial()
{
	m_bGameEnd = false;
	m_bEnemyCoolTime = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorial::~CTutorial()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CTutorial::Init()
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
	m_pMeshfieldBG = cRead.ReadMap("data/MAPTXT/maptutorial.txt", m_pBallastManager);

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


	m_pAdrenalineItemTutorial = new CAdrenalineItemTutorial;
	//�A�C�e��������
	if (FAILED(m_pAdrenalineItemTutorial->Init()))
	{
		return -1;
	}

	//�ׂ����ݒ�
	m_pAdrenalineItemTutorial->Set3DObject(cRead.ReadXFile("data/MODEL/�A�h���i����.x"), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pAdrenalineItemTutorial->SetParentPos(D3DXVECTOR3(261.2f, 0.0f, 2961.0f));

	m_pAdrenalineItemTutorial->SetLightVec(m_LightVec);

	//�A�C�e��������
	if (FAILED(CAdrenalineItem::AllInit()))
	{
		return -1;
	}

	CAdrenalineItem::AllSetLightVec(m_LightVec);

	//UI
	m_pUi = new C2DPolygon;

	//UI�̏�����
	if (FAILED(m_pUi->Init()))
	{
		return -1;
	}

	int nIndex = CTexture::LoadTexture("data/TEXTURE/�`���[�g���A��UI.png");
	m_pUi->SetTextIndex(nIndex);
	m_pUi->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pUi->SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_pUi->SetPolygon();



	//�G�l�~�[�̏��̐ݒ�
	EnemyInitData EnemyInitData;
	EnemyInitData.fmove = -50.0f;
	EnemyInitData.pos = D3DXVECTOR3(261.2f, 0.0f, 1785.0f);
	EnemyInitData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyInitData.type = ENEMY_03;

	//����
	m_pEnemyTutorial = (CEnemy_Tutorial*)m_pEnmeyManager->CreateEnemy(EnemyInitData);


	m_nEnemyCoolTime = 0;
	m_nItemCoolTime = 0;

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CTutorial::Uninit()
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

	//�A�h���i�����A�C�e��
	if (m_pAdrenalineItemTutorial != nullptr)
	{
		m_pAdrenalineItemTutorial->Uninit();
		delete m_pAdrenalineItemTutorial;
		m_pAdrenalineItemTutorial = nullptr;
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

	if (m_pUi != nullptr)
	{
		m_pUi->Uninit();
		delete m_pUi;
		m_pUi = nullptr;
	}

	CAdrenalineItem::AllUninit();

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

	CGauge::AllUninit();

	
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CTutorial::Update()
{
	if (m_bGameEnd)
	{
		return;
	}

	m_pMesh_Cylinder->Update(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�J�����̍X�V
	m_pCamera->Update();


	if (m_pEnemyTutorial != nullptr)
	{
		if (m_pEnemyTutorial->CheckLife())
		{
			m_bEnemyCoolTime = true;
			m_pEnemyTutorial = nullptr;
		}
	}

	if (m_bEnemyCoolTime)
	{
		if (m_nEnemyCoolTime > ENEMY_COOL_TIME)
		{
			//�G�l�~�[�̏��̐ݒ�
			EnemyInitData EnemyInitData;
			EnemyInitData.fmove = -50.0f;
			EnemyInitData.pos = D3DXVECTOR3(261.2f, 0.0f, 1785.0f);
			EnemyInitData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			EnemyInitData.type = ENEMY_03;

			//����
			m_pEnemyTutorial = (CEnemy_Tutorial*)m_pEnmeyManager->CreateEnemy(EnemyInitData);

			m_nEnemyCoolTime = 0;

			m_bEnemyCoolTime = false;
		}
		else
		{
			m_nEnemyCoolTime++;
		}
	}

	m_pEnmeyManager->Update();

	//�Q�[���G���h�ł͂Ȃ�������X�V
	if (!m_bGameEnd)
	{
		m_pPlayer->Update();
	}

	m_pBallastManager->Update();

	CInput *pInput = CInput::GetKey();

	CAdrenalineItem::AllUpdate();

	CMotionParts::ALLUpdate();

	//�A�h���i�����A�C�e���̍X�V
	if (m_pAdrenalineItemTutorial != nullptr)
	{
		m_pAdrenalineItemTutorial->Update();

		//�����蔻��
		if (m_pAdrenalineItemTutorial->PlayerCollision(m_pPlayer->GetPos()))
		{
			//�T�E���h
			PlaySound(SOUND_LABEL_SE_ADRENALINE);
			m_pPlayer->AddGauge();
			m_pAdrenalineItemTutorial->Uninit();
			delete m_pAdrenalineItemTutorial;
			m_pAdrenalineItemTutorial = nullptr;
		}
	}
	else
	{//�A�h���i�����A�C�e���̎�������

		if (m_nItemCoolTime > ITEM_COOL_TIME)
		{
			m_pAdrenalineItemTutorial = new CAdrenalineItemTutorial;
			//�A�C�e��������
			if (FAILED(m_pAdrenalineItemTutorial->Init()))
			{
				assert(false);
			}
			CRead cRead;
			//�ׂ����ݒ�
			m_pAdrenalineItemTutorial->Set3DObject(cRead.ReadXFile("data/MODEL/�A�h���i����.x"), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			m_pAdrenalineItemTutorial->SetParentPos(D3DXVECTOR3(261.2f, 0.0f, 2961.0f));

			m_pAdrenalineItemTutorial->SetLightVec(m_LightVec);

			m_nItemCoolTime = 0;
		}
		else
		{
			m_nItemCoolTime++;
		}
	}
	

	if (pInput->Trigger(KEY_DECISION))
	{
		m_bGameEnd = true;
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_TITLE);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CTutorial::Draw()
{
	//�J�����̍X�V
	m_pCamera->SetCamera();

	m_pMesh_Cylinder->Draw();

	m_pMeshfieldBG->Draw();

	CMotionParts::ALLDraw();

	m_pBallastManager->Draw();

	m_pEnmeyManager->Draw();

	//�A�h���i�����A�C�e���̕`��
	if (m_pAdrenalineItemTutorial != nullptr)
	{
		m_pAdrenalineItemTutorial->Draw();
	}

	CAdrenalineItem::AllDraw();

	//�Q�[�W
	CGauge::AllDraw();

	m_pPlayer->Draw();

	m_pUi->Draw();

}
