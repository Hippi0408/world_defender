//=============================================================================
//
// ���I���� [ballast_acquired.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "ballast_acquired.h"
#include "convenience_function.h"
#include "manager.h"
#include "game.h"
#include "meshfield.h"
#include "ballast_manager.h"
#include "tutorial.h"
#include <assert.h>


const float CBallast_Acquired::GRAVITY = -0.5f;
const float CBallast_Acquired::MOVE_ATTENUATION = 0.1f;
const float CBallast_Acquired::MOVE_MAGNIFICATION = 3.0f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CBallast_Acquired::CBallast_Acquired()
{
	m_Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFallenGroundOnce = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CBallast_Acquired::~CBallast_Acquired()
{
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CBallast_Acquired::Update()
{
	//�e�N���X�̍X�V
	CBallast::Update();

	if (m_bFallenGroundOnce)
	{
		return;
	}

	if (GetParentPos().y < 0.0f)
	{
		m_bFallenGroundOnce = true;
		SetPosMove(D3DXVECTOR3(0.0f,0.0f,0.0f));

		D3DXVECTOR3 pos = GetParentPos();
		SetParentPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));

		//�}�l�[�W���[����v���C���[�̏��̎擾
		CManager* pManager = GetManager();

		//���I�}�l�[�W���[�̎擾
		CBallast_Manager* pBallast_Manager = nullptr;

		//���b�V���t�B�[���h�̎擾
		CMeshfield* pMeshfield = nullptr;

		CTutorial* pTutorial = nullptr;
		CGame* pGame = nullptr;

		switch (pManager->GetCurrentMode())
		{
		case TYPE_TUTORIAL:

			//�`���[�g���A���̎擾
			pTutorial = (CTutorial*)pManager->GetGameObject();
			//���I�}�l�[�W���[�̎擾
			pBallast_Manager = pTutorial->GetBallast_Manager();
			//���b�V���t�B�[���h�̎擾
			pMeshfield = pTutorial->GetMeshfield();
			break;

		case TYPE_GAME:

			//�Q�[���̎擾
			pGame = (CGame*)pManager->GetGameObject();
			//���I�}�l�[�W���[�̎擾
			pBallast_Manager = pGame->GetBallast_Manager();
			//���b�V���t�B�[���h�̎擾
			pMeshfield = pGame->GetMeshfield();
			break;

		case TYPE_TITLE:
		case TYPE_RESULT:
		default:
			assert(false);
			break;
		}


		//�}�b�v�`�b�v�̔ԍ�
		int nMap = pMeshfield->CheckPosLocation(GetParentPos());
		//��L�̕ۑ�
		SetListNumber(nMap);

		//���X�g�̓���ւ�
		pBallast_Manager->ReplacementList(this, nMap);
	}
	else if(GetParentPos().y > 0.0f && !GetFloating())
	{
		D3DXVECTOR3 move = GetPosMove();

		move += (move - D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * MOVE_ATTENUATION;

		move.y += GRAVITY;

		SetPosMove(move);
	}
}

//*****************************************************************************
// �Ԃ���ԃx�N�g���̐ݒ�
//*****************************************************************************
void CBallast_Acquired::SetVec(D3DXVECTOR3 Vec)
{
	//�ۑ�
	m_Vec = Vec;

	//�ړ���
	D3DXVECTOR3 move = m_Vec * MOVE_MAGNIFICATION;

	SetPosMove(move);
}
