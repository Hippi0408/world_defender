//=============================================================================
//
// 処理 [light.cpp]
// Author : KOZUNA HIROHITO

//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "light.h"
#include "manager.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CLight::CLight()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CLight::~CLight()
{
}

//*****************************************************************************
//ライトの初期化
//*****************************************************************************
HRESULT CLight::Init(void)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	D3DXVECTOR3 vecDir[LIGHT_MAX];//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&m_alight, sizeof(m_alight));

	//ライトの種類を設定
	m_alight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[2].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[3].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_alight[0].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	m_alight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	m_alight[2].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	m_alight[3].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	//ライトの位置
	m_alight[0].Position = D3DXVECTOR3(0.0f, 200.0f, 100.0f);
	m_alight[1].Position = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
	m_alight[2].Position = D3DXVECTOR3(500.0f, 200.0f, 0.0f);
	m_alight[3].Position = D3DXVECTOR3(-500.0f, 200.0f, 0.0f);

	D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ライトの方向を設定
	vecDir[0] = posR - m_alight[0].Position;
	vecDir[1] = posR - m_alight[1].Position;
	vecDir[2] = posR - m_alight[2].Position;
	vecDir[3] = posR - m_alight[3].Position;

	//正規化する（大きさ１のベクトルにする）；
	D3DXVec3Normalize(&vecDir[0], &vecDir[0]);
	D3DXVec3Normalize(&vecDir[1], &vecDir[1]);
	D3DXVec3Normalize(&vecDir[2], &vecDir[2]);
	D3DXVec3Normalize(&vecDir[3], &vecDir[3]);
	m_alight[0].Direction = vecDir[0];
	m_alight[1].Direction = vecDir[1];
	m_alight[2].Direction = vecDir[2];
	m_alight[3].Direction = vecDir[3];

	//ライトを設定する
	pD3DDevice->SetLight(0, &m_alight[0]);
	pD3DDevice->SetLight(1, &m_alight[1]);
	pD3DDevice->SetLight(2, &m_alight[2]);
	pD3DDevice->SetLight(3, &m_alight[3]);

	//ライトを有効にする
	pD3DDevice->LightEnable(0, TRUE);
	pD3DDevice->LightEnable(1, TRUE);
	pD3DDevice->LightEnable(2, TRUE);
	pD3DDevice->LightEnable(3, TRUE);
	
	return S_OK;
}

//*****************************************************************************
//ライトの終了処理
//*****************************************************************************
void CLight::Uninit(void)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//ライトを無効にする
	pD3DDevice->LightEnable(0, FALSE);
	pD3DDevice->LightEnable(1, FALSE);
	pD3DDevice->LightEnable(2, FALSE);
	pD3DDevice->LightEnable(3, FALSE);
}

//*****************************************************************************
//ライトの更新処理
//*****************************************************************************
void CLight::Update(void)
{
}

//*****************************************************************************
//ライトの描画処理
//*****************************************************************************
void CLight::Draw()
{
}

//*****************************************************************************
//ライトのベクトル
//*****************************************************************************
D3DXVECTOR3 CLight::GetLightVec()
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_alight[0].Position;

	//正規化
	D3DXVec3Normalize(&vec,&vec);

	return vec;
}
