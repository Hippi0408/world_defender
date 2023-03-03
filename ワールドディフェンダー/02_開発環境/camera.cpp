//=============================================================================
//
// camera.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera.h"
#include "manager.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCamera::CCamera()
{
	m_nVibration = 1;
	m_nVibrationCntMax = 0;
	m_nVibrationCnt = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCamera::~CCamera()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CCamera::Uninit()
{
}

//*****************************************************************************
// 更新処理
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
// 描画処理
//*****************************************************************************
void CCamera::Draw()
{
}

//*****************************************************************************
// カメラのセット
//*****************************************************************************
void CCamera::SetCamera()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	D3DXMatrixLookAtLH(&m_mtxView, &(m_posV + m_AddVibration), &(m_posR + m_AddVibration), &m_vecU);

	//ビューマトリックスの設定
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//アスペクト比
		40.0f,//どこからどこまでカメラで表示するか
		60000.0f);

	//プロジェクションマトリックスの設定
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
