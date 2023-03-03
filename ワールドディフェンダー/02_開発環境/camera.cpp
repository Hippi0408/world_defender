//=============================================================================
//
// camera.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "camera.h"
#include "manager.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCamera::CCamera()
{
	m_nVibration = 1;
	m_nVibrationCntMax = 0;
	m_nVibrationCnt = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCamera::~CCamera()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CCamera::Uninit()
{
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CCamera::Update()
{
	if (m_nVibrationCnt < m_nVibrationCntMax)
	{
		m_nVibrationCnt++;

		m_AddVibration.x = (float)(rand() % m_nVibration);
		m_AddVibration.y = (float)(rand() % m_nVibration);
		m_AddVibration.z = (float)(rand() % m_nVibration);
	}
	else
	{
		m_AddVibration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CCamera::Draw()
{
}

//*****************************************************************************
// �J�����̃Z�b�g
//*****************************************************************************
void CCamera::SetCamera()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	D3DXMatrixLookAtLH(&m_mtxView, &(m_posV + m_AddVibration), &(m_posR + m_AddVibration), &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//�A�X�y�N�g��
		40.0f,//�ǂ�����ǂ��܂ŃJ�����ŕ\�����邩
		60000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

float CCamera::GetRot()
{
	D3DXVECTOR3 vec = m_posR - m_posV;

	float fAngle = atan2f(vec.x, vec.z);

	return fAngle;
}

void CCamera::SetVibration(int nVibrationCntMax, int nVibration)
{
	m_nVibration = nVibration + 1;

	m_nVibrationCntMax = nVibrationCntMax;

	m_nVibrationCnt = 0;

	//m_AddVibration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

bool CCamera::CheckVibration()
{
	if (m_nVibrationCnt < m_nVibrationCntMax)
	{
		return true;
	}

	return false;
}
