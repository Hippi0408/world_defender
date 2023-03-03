//=============================================================================
//
// statgauge.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "statgauge.h"
#include "gauge.h"
#include <assert.h>

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CStatGauge::CStatGauge()
{
	m_pStatGauge = nullptr;
	m_nCntRecovery = 0;
	m_nValue = 0;
	m_nTexture = 0;
	m_GaugeState = NormalGauge;
	m_bTextureChange = false;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TopPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CStatGauge::~CStatGauge()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CStatGauge::Init()
{
	m_pStatGauge = CGauge::Create();

	if (FAILED(m_pStatGauge->Init()))
	{
		return -1;
	}

	m_pStatGauge->SetFadeSpeed(0.1f);
	m_pStatGauge->SetTextIndex(m_nTexture);
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CStatGauge::Uninit()
{
	if (m_pStatGauge != nullptr)
	{
		m_pStatGauge = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CStatGauge::Update()
{
	if (m_GaugeState == GaugeBreak)
	{
		return;
	}

	if (m_nValue <= 0)
	{
		m_GaugeState = BreakTime;
	}

	m_nCntRecovery++;

	if (m_nCntRecovery >= m_nCntRecoveryMax)
	{
		m_nCntRecovery = 0;
		SetStatGauge(m_nRecovery);
	}

	// 増減とテクスチャのリンク
	if (m_bTextureChange)
	{
		IncreaseAndDecreaseAndTexture();
	}

	m_pStatGauge->Update();

	m_TopPos = m_Pos;

	m_TopPos.y -= m_pStatGauge->GetHeightElasticity();

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CStatGauge::Draw()
{
}

//*****************************************************************************
// ライフバーの作成
//*****************************************************************************
void CStatGauge::CreateStatBar(SStatGauge SStatGauge)
{
	if (m_pStatGauge != nullptr)
	{
		m_nRecovery = SStatGauge.nRecovery;
		m_nValue = SStatGauge.nValueMax;
		m_nValueMax = SStatGauge.nValueMax;
		m_nCntRecoveryMax = SStatGauge.nRecoveryCntMaqx;
		m_pStatGauge->SetUp(SStatGauge.pos, SStatGauge.rot);
		m_Pos = SStatGauge.pos;
		m_pStatGauge->SetDiagonalLine(SStatGauge.fXSize, 0.0f);
		m_pStatGauge->SetNumerics(SStatGauge.nValueMax, SStatGauge.nValueMax);
		m_pStatGauge->SetHeightElasticity(SStatGauge.fYSize);
		m_pStatGauge->SetHeightMax(SStatGauge.fYSize);
		m_pStatGauge->SetColor(SStatGauge.color);
		m_pStatGauge->SetTextIndex(0);
		m_pStatGauge->SetPolygon();
	}
}

//*****************************************************************************
// ダメージ処理
//*****************************************************************************
void CStatGauge::SetStatGauge(int nStatGauge)
{
	
	if (m_nValue + nStatGauge > m_nValueMax)
	{
		if (m_GaugeState == BreakTime)
		{
			m_GaugeState = NormalGauge;
		}
		m_nValue = m_nValueMax;
	}
	else if(m_nValue + nStatGauge < 0)
	{
		if (m_GaugeState == BreakTime)
		{
			m_GaugeState = GaugeBreak;
		}
		m_nValue = 0;
	}
	else
	{
		if (m_GaugeState == BreakTime)
		{
			m_pStatGauge->SetFlashing(10);
			if (nStatGauge < 0)
			{
				m_GaugeState = GaugeBreak;
				m_pStatGauge->ChangeNumerics(0);
			}
		}
		m_nValue += nStatGauge;
	}

	if (m_pStatGauge != nullptr)
	{
		m_pStatGauge->ChangeNumerics(m_nValue);
	}

	m_pStatGauge->Update();
}

//*****************************************************************************
// ゲージの状態取得
//*****************************************************************************
bool CStatGauge::GetGaugeState(GaugeState GaugeState)
{
	if (m_GaugeState == GaugeState)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// 増減とテクスチャのリンク
//*****************************************************************************
void CStatGauge::IncreaseAndDecreaseAndTexture()
{
	//ゲージの最大高さと現在の高さ
	float fHeight, fHeightMax;

	//現在の高さ
	fHeight = (float)m_nValue;

	//最大高さ
	fHeightMax = (float)m_nValueMax;


	//割合計算用
	float fRatio;

	//計算
	fRatio = fHeight / fHeightMax;

	//割合保存
	m_fRatio = fRatio;

	//現在のUVMin
	D3DXVECTOR2 UVMin = m_pStatGauge->GetUVSizeMin();

	//最大値の変更
	UVMin.y = 1.0f - fRatio;

	//保存
	m_pStatGauge->SetUVSizeMin(UVMin);

}

//*****************************************************************************
// ダメージ処理
//*****************************************************************************
bool CStatGauge::ValueCheck(int nCheck)
{
	if (m_nValue <= nCheck)
	{
		return true;
	}
	return false;
}

void CStatGauge::SetTextureNum(int nTexture)
{
	m_nTexture = nTexture;
	m_pStatGauge->SetTextIndex(m_nTexture);
}

