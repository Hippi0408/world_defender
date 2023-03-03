//=============================================================================
//
// enemy.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy.h"
#include "read.h"
#include "game.h"
#include "meshfield.h"
#include "motion_parts.h"
#include "player.h"
#include "input.h"
#include "object_type_list.h"
#include "convenience_function.h"
#include "enemy_manager.h"

#ifdef _DEBUG
#include "psychokinesis_area.h"
#endif // _DEBUG

const D3DXVECTOR3 CEnemy::INIT_POS = D3DXVECTOR3(1000.0f, 0.0f, -0.0f); 
const float CEnemy::MOVE_INERTIA = 0.1f; 
const float CEnemy::JUMP_INERTIA = 0.01f;
const float CEnemy::INIT_RADIUS = 1600.0f;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEnemy::CEnemy()
{
	CMovable_Obj::SetLife(INIT_LIFE);
	CMovable_Obj::SetRadius(INIT_RADIUS);
	m_Type = ENEMY_00;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEnemy::~CEnemy()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CEnemy::Init()
{
	//�e�N���X�̏�����
	if (FAILED(CMovable_Obj::Init()))
	{
		return -1;
	}


#ifdef _DEBUG
	//�T�C�R�L�l�V�X�G���A�̏��̊m��
	m_pPsychokinesis_Area = new CPsychokinesis_Area;

	//������
	if (FAILED(m_pPsychokinesis_Area->Init()))
	{
		return -1;
	}

#endif // _DEBUG

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CEnemy::Uninit()
{

#ifdef _DEBUG
	//�T�C�R�L�l�V�X�G���A�̉��
	if (m_pPsychokinesis_Area != nullptr)
	{
		//�I������
		m_pPsychokinesis_Area->Uninit();
		delete m_pPsychokinesis_Area;
		m_pPsychokinesis_Area = nullptr;
	}
#endif // _DEBUG

	//�e�N���X�̏I������
	CMovable_Obj::Uninit();
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CEnemy::Update()
{
	D3DXVECTOR3 pos = GetPos();
	CMotionParts::AllSetShadowPos(D3DXVECTOR3(pos.x, 2.0f, pos.z), GetMotionNum());

	D3DXVECTOR3 move = GetMove();


	if (pos.y < 0.0f)
	{
		move.y = 0.0f;
		pos.y = 0.0f;

		SetPos(pos);

	}
	else if(pos.y > 0.0f)
	{
		move.y += m_fMovingDistance;
	}
	
	
	SetMove(move);


	//�e�N���X�̍X�V
	CMovable_Obj::Update();
	
	

	
#ifdef _DEBUG
	//�T�C�R�L�l�V�X�G���A�̍X�V�iPos����j
	m_pPsychokinesis_Area->Update(GetPos());

	m_pPsychokinesis_Area->SetRadius(GetRadius());
#endif // _DEBUG

}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CEnemy::Draw()
{
	//�e�N���X�̕`��
	CMovable_Obj::Draw();

#ifdef _DEBUG
	//�T�C�R�L�l�V�X�G���A�̕`��
	m_pPsychokinesis_Area->Draw();
#endif // _DEBUG
}

//*****************************************************************************
// �I����������
//*****************************************************************************
bool CEnemy::IsUnused()
{
	//���C�t�̊m�F
	if (CMovable_Obj::CheckLife())
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// ���[�V�������f���̐ݒ菈��
//*****************************************************************************
void CEnemy::SetMotionModel()
{
}

void CEnemy::KnockBack(D3DXVECTOR3 vec)
{
}
