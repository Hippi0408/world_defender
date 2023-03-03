//=============================================================================
//
// gauge.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "gauge.h"
#include "2dpolygon.h"

CGauge *CGauge::m_pGauge[CGauge::MAX_GAUGE] = {};
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGauge::CGauge()
{
	m_nNumerics = 0;
	m_nNumericsMax = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGauge::~CGauge()
{
}

//=============================================================================
// 更新
//=============================================================================
void CGauge::Update()
{
	CElasticityPolygon::Update();
}

//=============================================================================
// 数値とゲージのサイズ設定
//=============================================================================
void CGauge::SetNumerics(int nInitialValue, int nMax)
{
	m_nNumerics = nInitialValue;
	m_nNumericsMax = nMax;
}

//=============================================================================
// 数値とゲージのサイズ変更
//=============================================================================
void CGauge::AddNumerics(int nAddNumerics)
{
	m_nNumerics += nAddNumerics;

	if (m_nNumerics > m_nNumericsMax)
	{
		m_nNumerics = m_nNumericsMax;
	}
	else if (m_nNumerics < 0)
	{
		m_nNumerics = 0;
	}
	
	float fSize = (float)m_nNumerics / (float)m_nNumericsMax;

	float fData = GetHeightMax();
	fData *= fSize;

	SetHeightElasticity(fData);

	SetPolygon();
}

void CGauge::ChangeNumerics(int nNumerics)
{
	m_nNumerics = nNumerics;

	if (m_nNumerics > m_nNumericsMax)
	{
		m_nNumerics = m_nNumericsMax;
	}
	else if (m_nNumerics < 0)
	{
		m_nNumerics = 0;
	}

	float fSize = (float)m_nNumerics / (float)m_nNumericsMax;

	float fData = GetHeightMax();
	fData *= fSize;

	SetHeightElasticity(fData);

	SetPolygon();
}

//=============================================================================
// ゲージの作成
//=============================================================================
CGauge *CGauge::Create()
{
	for (int nCnt = 0; nCnt < CGauge::MAX_GAUGE; nCnt++)
	{
		if (m_pGauge[nCnt] == nullptr)
		{
			m_pGauge[nCnt] = new CGauge;
			return m_pGauge[nCnt];
		}
	}
	return nullptr;
}

//=============================================================================
// すべての更新
//=============================================================================
void CGauge::AllUpdate()
{
	for (int nCnt = 0; nCnt < CGauge::MAX_GAUGE; nCnt++)
	{
		if (m_pGauge[nCnt] != nullptr)
		{
			m_pGauge[nCnt]->Update();
		}
	}
}

//=============================================================================
// すべての終了
//=============================================================================
void CGauge::AllUninit()
{
	for (int nCnt = 0; nCnt < CGauge::MAX_GAUGE; nCnt++)
	{
		if (m_pGauge[nCnt] != nullptr)
		{
			m_pGauge[nCnt]->Uninit();
			delete m_pGauge[nCnt];
			m_pGauge[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// すべての描画
//=============================================================================
void CGauge::AllDraw()
{
	for (int nCnt = 0; nCnt < CGauge::MAX_GAUGE; nCnt++)
	{
		if (m_pGauge[nCnt] != nullptr)
		{
			m_pGauge[nCnt]->Draw();
		}
	}
}