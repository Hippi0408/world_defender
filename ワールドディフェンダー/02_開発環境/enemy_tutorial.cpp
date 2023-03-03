//=============================================================================
//
// enemy_tutorial.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy_tutorial.h"
#include "read.h"
#include "game.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "player.h"
#include "input.h"
#include "object_type_list.h"
#include "convenience_function.h"
#include "enemy_manager.h"
#include "ballast_manager.h"
#include <assert.h>
#include "tutorial.h"
#include "adrenaline_item.h"

const D3DXVECTOR3 CEnemy_Tutorial::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f);
const float CEnemy_Tutorial::MOVE_INERTIA = 2.5f;
const float CEnemy_Tutorial::JUMP_INERTIA = 0.1f;
const float CEnemy_Tutorial::INIT_RADIUS = 100.0f;
const float CEnemy_Tutorial::SEARCH_RANGE = 1500.0f;
const D3DXVECTOR3 CEnemy_Tutorial::ENEMY_SIZE_MAX = D3DXVECTOR3(INIT_RADIUS, INIT_RADIUS, INIT_RADIUS);
const D3DXVECTOR3 CEnemy_Tutorial::ENEMY_SIZE_MIN = D3DXVECTOR3(-INIT_RADIUS, 0.0f, -INIT_RADIUS);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEnemy_Tutorial::CEnemy_Tutorial()
{
	m_nRandomMoveCnt = 0;
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEnemy_Tutorial::~CEnemy_Tutorial()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CEnemy_Tutorial::Init()
{

	//�e�N���X�̏�����
	if (FAILED(CEnemy::Init()))
	{
		return -1;
	}

	//���C�t�̐ݒ�
	SetLife(INIT_LIFE);

	//�����̈ʒu
	SetPos(INIT_POS);

	//�P�t���[���O�̈ʒu
	SetOldPos(INIT_POS);

	//�ړ��ʂ̏�����
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�����̏�����
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���[�V�������f���̐ݒ菈��
	SetMotionModel();

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CEnemy_Tutorial::Uninit()
{
	//���[�V����
	CMotionParts::DestructionMotionModel(GetMotionNum());

	//�e�N���X�̏I������
	CEnemy::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CEnemy_Tutorial::Update()
{
	//�e�N���X�̍X�V����
	CEnemy::Update();

	//���[�V����
	CMotionParts::MoveMotionModel(GetMotionNum(), 0, &GetPos(), &GetRot());

}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CEnemy_Tutorial::Draw()
{
	//�e�N���X�̕`�揈��
	CEnemy::Draw();
}

//*****************************************************************************
// �I����������
//*****************************************************************************
bool CEnemy_Tutorial::IsUnused()
{
	if (CMovable_Obj::CheckLife())
	{
		CAdrenalineItem::DropAdrenalineItem(GetPos());
		return true;
	}

	return false;
}

//*****************************************************************************
// ���[�V�������f���̐ݒ菈��
//*****************************************************************************
void CEnemy_Tutorial::SetMotionModel()
{
	CMotionParts::AllSetObject_Type_List(GetMotionNum(), OBJ_ENEMY);
}

void CEnemy_Tutorial::KnockBack(D3DXVECTOR3 vec)
{
}
