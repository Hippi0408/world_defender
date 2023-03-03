//=============================================================================
//
// ���I�̊Ǘ��ҏ��� [ballast_manager.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "ballast_manager.h"
#include "ballast.h"
#include <assert.h>
#include "game.h"
#include "manager.h"
#include "meshfield.h"
#include "convenience_function.h"
#include "enemy_manager.h"
#include "read.h"
#include "ballast_acquired.h"
#include "sound.h"
#include "tutorial.h"
#include "tps_camera.h"

const float CBallast_Manager::MAP_MAX = 15000.0f;
const D3DXVECTOR3 CBallast_Manager::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const float CBallast_Manager::BASE_RADIUS_PER_RUBBLE_ONE = 500.0f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBallast_Manager::CBallast_Manager()
{
	m_nPlListNumber = 0;
	m_nMeshfieldNumMax = 0;
	m_pMeshfieldCopy = nullptr;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBallast_Manager::~CBallast_Manager()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CBallast_Manager::Init()
{
	m_pBallastListData = nullptr;
	m_nBallastListDataMax = 0;
	m_nPlListNumber = 0;
	m_nMeshfieldNumMax = 0;
	m_pMeshfieldCopy = nullptr;

	CRead Read;
	m_nBallast_Acquired_Model[0] = Read.ReadXFile("data/MODEL/���I01.x");
	m_nBallast_Acquired_Model[1] = Read.ReadXFile("data/MODEL/���I02.x");
	m_nBallast_Acquired_Model[2] = Read.ReadXFile("data/MODEL/���I03.x");
	m_nBallast_Acquired_Model[3] = Read.ReadXFile("data/MODEL/���I04.x");
	m_nBallast_Acquired_Model[4] = Read.ReadXFile("data/MODEL/���I05.x");

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CBallast_Manager::Uninit()
{
	//���X�g�̉��
	for (int nCnt = 0; nCnt < m_nBallastListDataMax; nCnt++)
	{
		//�C�e���[�^�[���[�v
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end();)
		{
			//�C�e���[�^�[���犢�I�̃|�C���^�̑��
			CBallast* pBallast = *itr;

			//���I�|�C���^�̉��
			if (pBallast != nullptr)
			{
				//�I������
				pBallast->Uninit();
				delete pBallast;
				pBallast = nullptr;
			}

			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_pBallastListData[nCnt].erase(itr);
		}
	}

	//���I�S�̃|�C���^�̉��
	delete[] m_pBallastListData;
	m_pBallastListData = nullptr;
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CBallast_Manager::Update()
{
	//�}�l�[�W���[����v���C���[�̏��̎擾
	CManager* pManager = GetManager();

	//�G�l�~�[�}�l�[�W���[�̎擾
	CEnemy_Manager* pEnemy_Manager = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:
	case TYPE_GAME:
		//�G�l�~�[�Ƃ̓����蔻��
		CollisionEnemy();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
		break;
	default:
		assert(false);
		break;
	}

	//���X�g�̍X�V
	for (int nCnt = 0; nCnt < m_nBallastListDataMax; nCnt++)
	{
		//�C�e���[�^�[���[�v
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); )
		{
			//�C�e���[�^�[���犢�I�̃|�C���^�̑��
			CBallast* pBallast = *itr;

			//���I�|�C���^�̍X�V����
			if (pBallast == nullptr)
			{
				assert(false);
			}

			//�g�p��Ԃ��ǂ���
			if (pBallast->GetUse())
			{
				D3DXVECTOR3 pos = pBallast->GetPos() + pBallast->GetParentPos();
				//�G���A�O�ɂ��邩�ǂ���
				if (CConvenience_Function::CircleCollision(D3DXVECTOR3(0.0f,0.0f,0.0f), MAP_MAX, pos, 0.0f))
				{
					//�X�V����
					pBallast->Update();

					//�C�e���[�^�[��i�߂�
					itr++;

					continue;
				}
			}
			
			
			//�I������
			pBallast->Uninit();

			//�j��
			delete pBallast;
			pBallast = nullptr;

			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_pBallastListData[nCnt].erase(itr);
		}
	}

	
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CBallast_Manager::Draw()
{

	//�}�l�[�W���[����v���C���[�̏��̎擾
	CManager* pManager = GetManager();

	//�J�����̃|�C���^�[
	CTpsCamera* pTpsCamera = nullptr;

	CTutorial* pTutorial = nullptr;
	CGame* pGame = nullptr;

	//�����_�Ǝ��_
	D3DXVECTOR3 posR, posV;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//�`���[�g���A���̎擾
		pTutorial = (CTutorial*)pManager->GetGameObject();
		//�J�����̎擾
		pTpsCamera = (CTpsCamera*)pTutorial->GetCamera();

		posR = pTpsCamera->GetPosR();
		posV = pTpsCamera->GetPosV();
		break;

	case TYPE_GAME:

		//�Q�[���̎擾
		pGame = (CGame*)pManager->GetGameObject();
		//�J�����̎擾
		pTpsCamera = (CTpsCamera*)pGame->GetCamera();

		posR = pTpsCamera->GetPosR();
		posV = pTpsCamera->GetPosV();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
		posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	default:
		assert(false);
		break;
	}


	//���X�g�̍X�V
	for (int nCnt = 0; nCnt < m_nMeshfieldNumMax; nCnt++)
	{
		//�C�e���[�^�[���[�v
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); itr++)
		{
			//�C�e���[�^�[���犢�I�̃|�C���^�̑��
			CBallast* pBallast = *itr;

			//���I�̕`��
			if (pBallast != nullptr)
			{
				if (pBallast->IsExistingDuring2Point(posR, posV))
				{
					pBallast->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 0.0f);
					pBallast->SetWithinRangeColor(true);
				}
				else
				{
					pBallast->SetWithinRangeColor(false);
				}

				//�`�揈��
				pBallast->Draw();
			}
		}
	}
}

//*****************************************************************************
//���I�̐ݒ�
//*****************************************************************************
void CBallast_Manager::SetBallast(int nNumber, Object_Data Data)
{
	if (nNumber < 0 || nNumber >= m_nBallastListDataMax)
	{
		return;
	}

	//���I�N���X�̐���
	CBallast* pBallast = new CBallast;

	//������
	if (FAILED(pBallast->Init()))
	{
		assert(false);
	}

	//���b�V�����̈ʒu�i�ԍ��j
	pBallast->SetListNumber(nNumber);

	//�g�p���郂�f���ԍ��A���I�̈ʒu�A���I�̌���
	pBallast->Set3DObject(Data.nPattn, D3DXVECTOR3(0.0f,0.0f,0.0f), Data.rot);
	pBallast->SetParentPos(Data.pos);

	//�@���ݒ�
	pBallast->SetNormal();

	//���a�ݒ�
	pBallast->SetRadius();

	//�e�̐ݒ�
	pBallast->SetLightVec(m_Light);

	//���X�g�Ɋ��I����ǉ�
	m_pBallastListData[nNumber].push_back(pBallast);
}

//*****************************************************************************
//���b�V���t�B�[���h�̐ݒ�
//*****************************************************************************
void CBallast_Manager::MeshfieldSet(CMeshfield* pMeshfield)
{
	//�ۑ����Null�`�F�b�N
	if (m_pMeshfieldCopy != nullptr)
	{
		assert(false);
	}

	//�Q�[������n�ʁi���b�V���j�̎擾
	m_pMeshfieldCopy = pMeshfield;

	//���b�V���̃}�X�̐��̍ő�̐ݒ�
	m_nMeshfieldNumMax = m_pMeshfieldCopy->GetMeshfieldNumMax();

}

//*****************************************************************************
//�T�C�R�L�l�V�X�G���A�ɂ������炻�̃I�u�W�F�N�g�̃|�C���^��Ԃ�
//*****************************************************************************
CBallast * CBallast_Manager::CheckCircleCollision(D3DXVECTOR3 pos, float fRadius, int nAdrenaline)
{
	//���X�g�̍X�V
	for (int nCnt = 0; nCnt < m_nMeshfieldNumMax; nCnt++)
	{
		//�C�e���[�^�[���[�v
		for (auto itr = m_pBallastListData[nCnt].begin(); itr != m_pBallastListData[nCnt].end(); itr++)
		{
			//�C�e���[�^�[���犢�I�̃|�C���^�̑��
			CBallast* pBallast = *itr;

			//���INULL�`�F�b�N
			if (pBallast == nullptr)
			{
				assert(false);
			}

			//�g�p��Ԃ��ǂ���
			if (!pBallast->GetUse())
			{
				//�����𖳎�����
				continue;
			}

			//���V��Ԃ��ǂ���
			if (pBallast->GetFloating())
			{
				continue;
			}

			//�T�C�Y�|�C���g�̎擾
			int nSizePoint = pBallast->GetSizePoint();
			
			//���̕��𕂂�����邩�ǂ���
			if (nSizePoint > nAdrenaline)
			{
				//�����𖳎�����
				continue;
			}

			//�T�C�R�L�l�V�X�G���A�ɂ��邩�ǂ���
			if (CConvenience_Function::CircleCollision(pos,fRadius, pBallast->GetParentPos(), 0.0f))
			{
				return pBallast;
			}

		}
	}

	return nullptr;
}

//*****************************************************************************
//�T�C�R�L�l�V�X�G���A�ɂ������炻�̃I�u�W�F�N�g�̐F��ς���
//*****************************************************************************
void CBallast_Manager::WithinRangeColor(int nMapGrid, D3DXVECTOR3 pos, float fRadius, int nAdrenaline)
{
	if (nMapGrid < 0 || nMapGrid >= m_nBallastListDataMax)
	{
		return;
	}

	//�C�e���[�^�[���[�v
	for (auto itr = m_pBallastListData[nMapGrid].begin(); itr != m_pBallastListData[nMapGrid].end(); itr++)
	{
		//�ϐ��錾
		D3DXVECTOR3 Extrusion = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pBallast = *itr;

		//���INULL�`�F�b�N
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//�g�p��Ԃ��ǂ���
		if (!pBallast->GetUse())
		{
			//�����𖳎�����
			continue;
		}

		//���V��Ԃ��ǂ���
		if (pBallast->GetFloating())
		{
			continue;
		}

		//�T�C�Y�|�C���g�̎擾
		int nSizePoint = pBallast->GetSizePoint();

		//���̕��𕂂�����邩�ǂ���
		if (nSizePoint > nAdrenaline)
		{
			pBallast->SetWithinRangeColor(false);
			//�����𖳎�����
			continue;
		}

		//�T�C�R�L�l�V�X�G���A�ɂ��邩�ǂ���pBallast->GetVtxMax().x
		if (CConvenience_Function::CircleCollision(pos, fRadius, pBallast->GetParentPos(), 0.0f))
		{
			pBallast->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 0.05f);
			pBallast->SetWithinRangeColor(true);
		}
		else
		{
			pBallast->SetWithinRangeColor(false);
		}
	}
}

//*****************************************************************************
//��Q���̓����蔻��
//*****************************************************************************
D3DXVECTOR3 CBallast_Manager::CollisionBallast(int nMapGrid, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos, D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	D3DXVECTOR3 Add = pos;

	if (nMapGrid < 0 || nMapGrid >= m_nBallastListDataMax)
	{
		return Add;
	}

	//�C�e���[�^�[���[�v
	for (auto itr = m_pBallastListData[nMapGrid].begin(); itr != m_pBallastListData[nMapGrid].end(); itr++)
	{
		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pBallast = *itr;

		//���INULL�`�F�b�N
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//���V��Ԃ��ǂ���
		if (pBallast->GetFloating())
		{
			continue;
		}

		//�ϐ��錾
		D3DXVECTOR3 Extrusion = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����o������
		Extrusion = pBallast->ConclusionCollision(pos, oldpos, max, min);

		//�����o�������������ǂ���
		if (Extrusion != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			//�����o���ϐ��ɓ����
			Add = Extrusion;

			//for���𔲂���
			break;
		}

	}

	//�����o���l��Ԃ�
	return Add;
}

//*****************************************************************************
//�G�l�~�[�Ƃ̓����蔻��
//*****************************************************************************
void CBallast_Manager::CollisionEnemy()
{
	//�}�l�[�W���[����v���C���[�̏��̎擾
	CManager* pManager = GetManager();

	//�G�l�~�[�}�l�[�W���[�̎擾
	CEnemy_Manager* pEnemy_Manager = nullptr;

	CTutorial* pTutorial = nullptr;
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//�`���[�g���A���̎擾
		pTutorial = (CTutorial*)pManager->GetGameObject();
		//�G�l�~�[�}�l�[�W���[�̎擾
		pEnemy_Manager = pTutorial->GetEnemy_Manager();
		break;

	case TYPE_GAME:

		//�Q�[���̎擾
		pGame = (CGame*)pManager->GetGameObject();
		//�G�l�~�[�}�l�[�W���[�̎擾
		pEnemy_Manager = pGame->GetEnemy_Manager();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	//�C�e���[�^�[���[�v
	for (auto itr = m_FloatingBallstList.begin(); itr != m_FloatingBallstList.end();)
	{
		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pBallast = *itr;

		//���INULL�`�F�b�N
		if (pBallast == nullptr)
		{
			assert(false);
		}

		//�g�p��Ԃ��ǂ���
		if (!pBallast->GetUse())
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�����𖳎�����
			continue;
		}

		//���V��Ԃ��ǂ���
		if (pBallast->GetFloating())
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�����𖳎�����
			continue;
		}

		//����
		bool bHit = pEnemy_Manager->EnemyCollision(pBallast->GetPos() + pBallast->GetParentPos(), pBallast->GetRadius());

		if (!bHit)
		{
			//�C�e���[�^�[��i�߂�
			itr++;

			//�����𖳎�����
			continue;
		}

		//�T�E���h
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		
		D3DXVECTOR3 vec = pBallast->GetPosMove() * -1.0f;

		D3DXVec3Normalize(&vec, &vec);

		SetBallastAcquired(vec, pBallast->GetParentPos() + pBallast->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), pBallast->GetRadius());

		//���I�̎g�p��Ԃ�ύX
		pBallast->SetUse(false);

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_FloatingBallstList.erase(itr);

	}

}

//*****************************************************************************
//���V��Ԃ̃��X�g�ǉ�
//*****************************************************************************
void CBallast_Manager::SetFloatingBallst(CBallast * pBallast)
{
	//���X�g�Ɋ��I����ǉ�
	m_FloatingBallstList.push_back(pBallast);

	pBallast->SetWithinRangeColor(false);
}

//*****************************************************************************
//��V�I���I�̐���(�����͔�΂����������x�N�g��,���Ƃ͊�{���)
//*****************************************************************************
void CBallast_Manager::SetBallastAcquired(D3DXVECTOR3 vec, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	if (BASE_RADIUS_PER_RUBBLE_ONE > fRadius)
	{
		return;
	}

	int nBallastNum = (int)fRadius / BASE_RADIUS_PER_RUBBLE_ONE;

	for (int nCnt = 0; nCnt < nBallastNum; nCnt++)
	{
		//�}�b�v�`�b�v�̈ʒu
		int nNumber = 0;

		//������ԕ���
		D3DXVECTOR3 Vec = vec;

		//��V�I���I�N���X�̐���
		CBallast_Acquired* pBallastAcquired = new CBallast_Acquired;

		//������
		if (FAILED(pBallastAcquired->Init()))
		{
			assert(false);
		}

		//�}�b�v�`�b�v�̈ʒu�m�F
		nNumber = m_pMeshfieldCopy->CheckPosLocation(pos);

		if (nNumber < 0 || nNumber >= m_nBallastListDataMax)
		{
			return;
		}

		//���b�V�����̈ʒu�i�ԍ��j
		pBallastAcquired->SetListNumber(nNumber);

		//�g�p���郂�f���ԍ��A���I�̈ʒu�A���I�̌���
		pBallastAcquired->Set3DObject(m_nBallast_Acquired_Model[rand() % BALLAST_PATTERN], D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot);

		pBallastAcquired->SetParentPos(pos);

		//������ԕ���(�������܂߂�)
		Vec.x += (float)(rand() % 20 - 10);
		Vec.y += (float)(rand() % 20 - 10);
		Vec.z += (float)(rand() % 20 - 10);

		//�m�[�}���C�Y
		D3DXVec3Normalize(&Vec, &Vec);

		//������ԕ����̐ݒ�
		pBallastAcquired->SetVec(Vec);

		//�@���ݒ�
		pBallastAcquired->SetNormal();

		//���a�ݒ�
		pBallastAcquired->SetRadius();

		//�e�̐ݒ�
		pBallastAcquired->SetLightVec(m_Light);

		//���X�g�Ɋ��I����ǉ�
		m_pBallastListData[nNumber].push_back(pBallastAcquired);
	}
}

//*****************************************************************************
//���X�g���̓���ւ�
//*****************************************************************************
void CBallast_Manager::ReplacementList(CBallast * pBallast, int nNext)
{
	//�Ō�ɓo�^����Ă����ԍ�
	int nLastNum = pBallast->GetListNumber();

	if (nNext == nLastNum)
	{
		return;
	}

	//�C�e���[�^�[���[�v
	for (auto itr = m_pBallastListData[nLastNum].begin(); itr != m_pBallastListData[nLastNum].end();)
	{
		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CBallast* pballast = *itr;

		//���INULL�`�F�b�N
		if (pballast == nullptr)
		{
			assert(false);
		}

		//�w��̃|�C���^���ǂ���
		if (pBallast == pballast)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_pBallastListData[nLastNum].erase(itr);

			//�����𔲂���
			break;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}

	//���X�g�Ɋ��I����ǉ�
	m_pBallastListData[nNext].push_back(pBallast);
}

//*****************************************************************************
//���X�g�̌����m��
//*****************************************************************************
void CBallast_Manager::CreateBallastListData(int nDataNum)
{
	//�f�[�^�̊m��
	m_pBallastListData = new std::list<CBallast*>[nDataNum];

	//Max�̕ۑ�
	m_nBallastListDataMax = nDataNum;
}

