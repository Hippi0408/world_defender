//=============================================================================
//
// ufo.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "ufo.h"
#include <assert.h>
#include "convenience_function.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "enemy_manager.h"

const float CUfo::POP_RANGE = 30000.0f;
const float CUfo::MOVING_DISTANCE = 30.0f;
const float CUfo::POP_POS_Y = 10000.0f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CUfo::CUfo()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CUfo::~CUfo()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CUfo::Init()
{
	m_nMoveCounter = MOVE_COUNTER_MAX;

	m_nPopInterval = 0;

	//�e�N���X�̏�����
	if (FAILED(C3DObject::Init()))
	{
		assert(false);
	}

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CUfo::Uninit()
{
	//�e�N���X�̏I������
	C3DObject::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CUfo::Update()
{
	//UFO���G���A�O�ɏo�����ǂ���
	bool bOutsideArea = CConvenience_Function::CircleCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), POP_RANGE, GetParentPos(), 0.0f);

	if (m_nMoveCounter < MOVE_COUNTER_MAX)
	{
		if (!bOutsideArea)
		{
			m_nMoveCounter++;
			return;
		}
	}
	else
	{
		m_nMoveCounter = 0;

		float fAngle = D3DXToRadian(rand() % 360);

		D3DXVECTOR3 Pop = D3DXVECTOR3(0.0f, POP_POS_Y, 0.0f);
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		Pop.x = cosf(fAngle) * POP_RANGE;
		Pop.z = sinf(fAngle) * POP_RANGE;

		SetParentPos(Pop);

		Move = Pop * -1.0f;

		Move.y = 0.0f;

		D3DXVec3Normalize(&Move, &Move);

		Move *= MOVING_DISTANCE;

		SetPosMove(Move);

	}


	//�e���̎擾
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move = GetPosMove();
	D3DXVECTOR3 rot = GetRot();

	//�ʒu�̍X�V
	pos = GetParentPos();
	pos += move;
	SetParentPos(pos);

	rot.y += D3DXToRadian(5.0f);

	rot = CConvenience_Function::NormalizationRot(rot);

	SetRot(rot);

	//�e�N���X�̍X�V����
	C3DObject::Update();

	// �G���G�̏o������
	EnemyPop();
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CUfo::Draw()
{
	//�}�g���b�N�X�̌v�Z
	CalculationMatrix();
	//�e�N���X�̕`�揈��
	C3DObject::Draw();
}

//*****************************************************************************
// �G���G�̏o������
//*****************************************************************************
void CUfo::EnemyPop()
{
	if (m_nPopInterval < POP_INTERVAL_MAX)
	{
		m_nPopInterval++;
		return;
	}

	m_nPopInterval = 0;

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

	//�G�l�~�[�̏��̐ݒ�
	EnemyInitData EnemyInitData;
	EnemyInitData.fmove = -50.0f;
	EnemyInitData.pos = GetParentPos();
	EnemyInitData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyInitData.type = ENEMY_01;

	//�G�l�~�[�̏o��
	pEnemy_Manager->CreateEnemy(EnemyInitData);

}
