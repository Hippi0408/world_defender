//=============================================================================
//
// texture.h
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "texture.h"
#include "manager.h"

int CTexture::m_nNowIndex = 0;
CTexture * CTexture::m_pTextuerClass = nullptr;
LPDIRECT3DTEXTURE9 CTexture::m_pTexture[TEXTURE_MAX] = {};

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTexture::CTexture()
{
	m_nNowIndex = 0;
	m_pTextuerClass = nullptr;
	ZeroMemory(m_pTexture, sizeof(m_pTexture));
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTexture::~CTexture()
{
}

//*****************************************************************************
// テクスチャの生成
//*****************************************************************************
CTexture *CTexture::Create()
{
	m_pTextuerClass = new CTexture;

	m_pTexture[0] = nullptr;
	m_nNowIndex++;

	return m_pTextuerClass;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTexture::Uninit()
{
	for (int nCnt = 0;nCnt < m_nNowIndex; nCnt++)
	{
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = nullptr;
		}
	}

	if (m_pTextuerClass != nullptr)
	{
		delete m_pTextuerClass;
		m_pTextuerClass = nullptr;
	}
}

//*****************************************************************************
// テクスチャのロード
//*****************************************************************************
int CTexture::LoadTexture(char *address)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice,
		address,
		&m_pTexture[m_nNowIndex]);

	int nIndex = m_nNowIndex;
	m_nNowIndex++;
	return nIndex;
}
