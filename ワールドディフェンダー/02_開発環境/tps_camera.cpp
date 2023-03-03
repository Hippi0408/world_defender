//=============================================================================
//
// tps_camera.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "tps_camera.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "convenience_function.h"
#include "tutorial.h"
#include <assert.h>

const float CTpsCamera::DISTANCE_MAX = 3500.0f;
const D3DXVECTOR3 CTpsCamera::RANGE_WITH_PLAYER_V = D3DXVECTOR3(100.0f, 0.0f, -500.0f);
//const D3DXVECTOR3 CTpsCamera::RANGE_WITH_PLAYER_R = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
const D3DXVECTOR3 CTpsCamera::RANGE_WITH_PLAYER_R = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTpsCamera::CTpsCamera()
{
	m_CameraVectorConversion = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPlayerDistance = 0.0f;
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VPos = RANGE_WITH_PLAYER_V;
	m_fDistance = 0.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTpsCamera::~CTpsCamera()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CTpsCamera::Init()
{
	m_VPos = RANGE_WITH_PLAYER_V;
	m_fPlayerDistance = D3DXVec3Length(&RANGE_WITH_PLAYER_V);
	D3DXVECTOR3 vec = RANGE_WITH_PLAYER_V - RANGE_WITH_PLAYER_R;
	D3DXVec3Normalize(&vec, &vec);
	m_CameraVec = vec;
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_DestPos = RANGE_WITH_PLAYER_V;

	//�e�N���X��Init
	if (FAILED(CCamera::Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CTpsCamera::Update()
{
	CCamera::Update();

	//���̓f�o�C�X�̎擾
	CInput* pInput = CInput::GetKey();

	D3DXVECTOR3 MouseMove;
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	MouseMove = pInput->GetMouseMove();

	MouseMove = D3DXVECTOR3(MouseMove.y, MouseMove.x, 0.0f);

	if(D3DXVec3Length(&MouseMove) > 1.0f)
	{
		D3DXVec3Normalize(&MouseMove,&MouseMove);

		rot = MouseMove * (D3DX_PI / 180.0f);

		rot.x *= 2.0f;
		rot.y *= 3.0f;
		rot.z *= 3.0f;
	}

	m_Rot += rot;

	if (m_Rot.x  > D3DXToRadian(80))
	{
		m_Rot.x = D3DXToRadian(80);
	}
	else if (m_Rot.x  < D3DXToRadian(-50))
	{
		m_Rot.x = D3DXToRadian(-50);
	}

	rot = CConvenience_Function::NormalizationRot(m_Rot);
	m_Rot.y = rot.y;
	m_Rot.z = rot.z;

	//�}�l�[�W���[����v���C���[�̏��̎擾
	CManager* pManager = GetManager();

	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;

	CTutorial* pTutorial = nullptr;
	CGame* pGame = nullptr;

	switch (pManager->GetCurrentMode())
	{
	case TYPE_TUTORIAL:

		//�`���[�g���A���̎擾
		pTutorial = (CTutorial*)pManager->GetGameObject();

		//�v���C���[�̎擾
		pPlayer = pTutorial->GetPlayer();
		break;

	case TYPE_GAME:

		//�Q�[���̎擾
		pGame = (CGame*)pManager->GetGameObject();

		//�v���C���[�̎擾
		pPlayer = pGame->GetPlayer();
		break;

	case TYPE_TITLE:
	case TYPE_RESULT:
	default:
		assert(false);
		break;
	}

	D3DXMATRIX mtxRot, mtxTrans,mtxworld;		//�v�Z�p�̃}�g���b�N�X
	D3DXVECTOR3 PlPos = pPlayer->GetPos();

	PlPos.y += 100.0f;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxworld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxworld, &mtxworld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, PlPos.x, PlPos.y, PlPos.z);
	D3DXMatrixMultiply(&mtxworld, &mtxworld, &mtxTrans);

	D3DXVECTOR3 posV, posR,add;

	posV.x = m_VPos.x + m_CameraVec.x * m_fDistance;
	posV.y = m_VPos.y + m_CameraVec.y * m_fDistance;
	posV.z = m_VPos.z + m_CameraVec.z * m_fDistance;

	D3DXVec3TransformCoord(&posV, &posV, &mtxworld);
	D3DXVec3TransformCoord(&posR, &RANGE_WITH_PLAYER_R, &mtxworld);
	D3DXVec3TransformCoord(&m_CameraVectorConversion, &m_CameraVec, &mtxworld);
	D3DXVec3Normalize(&m_CameraVectorConversion, &m_CameraVectorConversion);

	//�����_�̕ۑ�
	SetPosR(posR);



	if (posV.y < 10.0f)
	{
		float fPosR = 10.0f - posV.y;
		AddPosR(D3DXVECTOR3(0.0f, fPosR, 0.0f));
		posV.y = 10.0f;
	}


	m_DestPos = posV;

	add = (m_DestPos - GetPosV()) * 1.0f;

	//���_�̕ۑ�
	AddPosV(add);
	
}

//*****************************************************************************
// �J�����̃x�N�g���̎擾
//*****************************************************************************
D3DXVECTOR3 CTpsCamera::GetCameraVec()
{
	//return D3DXVECTOR3(m_CameraVectorConversion.x, -m_CameraVectorConversion.y, m_CameraVectorConversion.z);
	return m_Rot;
}

void CTpsCamera::RateCalculation(float fRate)
{
	m_fDistance = DISTANCE_MAX * fRate;
}
