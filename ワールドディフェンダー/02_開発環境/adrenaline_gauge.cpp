//=============================================================================
//
// アドレナリンゲージ処理 [adrenaline_gauge.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "adrenaline_gauge.h" 
#include "texture.h"
#include "2d6polygon.h"
#include "statgauge.h"

const D3DXVECTOR3 CAdrenaline_Gauge::INIT_POS = D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 400.0f, 0.0f);
const float CAdrenaline_Gauge::INIT_HEIGHT = 700.0f;
const float CAdrenaline_Gauge::INIT_WIDTH = 180.0f;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAdrenaline_Gauge::CAdrenaline_Gauge()
{
	m_Pos = INIT_POS;
	m_nGauge = 0;
	m_nGaugeDest = 0;
	m_pGauge = nullptr;
	m_pAdrenaline_Gauge = nullptr;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAdrenaline_Gauge::~CAdrenaline_Gauge()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAdrenaline_Gauge::Init()
{
	//アドレナリンゲージBG
	m_pAdrenaline_Gauge = new CStatGauge;
	if (FAILED(m_pAdrenaline_Gauge->Init()))
	{
		return -1;
	}

	CStatGauge::SStatGauge StatGauge;

	StatGauge.pos = D3DXVECTOR3(INIT_POS.x, INIT_POS.y + INIT_HEIGHT * 0.5f, 0.0f);
	StatGauge.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	StatGauge.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	StatGauge.fXSize = INIT_WIDTH;
	StatGauge.fYSize = INIT_HEIGHT;
	StatGauge.nValueMax = MAX_GAUGE;
	StatGauge.nRecoveryCntMaqx = 0;
	StatGauge.nRecovery = 0;
	m_pAdrenaline_Gauge->CreateStatBar(StatGauge);
	m_pAdrenaline_Gauge->SetValue(1);
	m_pAdrenaline_Gauge->SetTextureNum(CTexture::LoadTexture("data/TEXTURE/タイムゲージ.png"));
	m_pAdrenaline_Gauge->SetTextureChange(true);

	//情報の確保
	m_pGauge = new C2D6Polygon;

	//初期化
	if (FAILED(m_pGauge->Init()))
	{
		return -1;
	}

	int nIndex = CTexture::LoadTexture("data/TEXTURE/アドレナリンゲージ.png");
	m_pGauge->SetTextIndex(nIndex);
	m_pGauge->SetPos(INIT_POS);
	m_pGauge->SetDiagonalLine(INIT_WIDTH, INIT_HEIGHT);
	m_pGauge->SetPolygon();


	return S_OK;
}

//*****************************************************************************
// 初期化(Posあり)
//*****************************************************************************
HRESULT CAdrenaline_Gauge::Init(D3DXVECTOR3 Pos)
{
	//位置の保管
	m_Pos = Pos;

	//初期化
	if (FAILED(Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CAdrenaline_Gauge::Uninit()
{
	//情報の解放
	if (m_pAdrenaline_Gauge != nullptr)
	{
		//終了処理
		m_pAdrenaline_Gauge->Uninit();
		delete m_pAdrenaline_Gauge;
		m_pAdrenaline_Gauge = nullptr;
	}

	//情報の解放
	if (m_pGauge != nullptr)
	{
		//終了処理
		m_pGauge->Uninit();
		delete m_pGauge;
		m_pGauge = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CAdrenaline_Gauge::Update()
{

	m_nCounter++;
	if (m_nCounter > COUNTER_MAX)
	{
		m_nCounter = 0;

		if (m_nGaugeDest > MAX_GAUGE)
		{
			m_nGaugeDest = MAX_GAUGE;
		}
		else if (m_nGaugeDest < 0)
		{
			m_nGaugeDest = 0;
		}
		else if (m_nGaugeDest > 0)
		{
			m_nGaugeDest -= AUTOMATIC_DECREASE;
		}
		
	}
	
	m_pAdrenaline_Gauge->SetValue(m_nGauge+1);

	m_pAdrenaline_Gauge->Update();

	if (m_nGaugeDest > m_nGauge)
	{
		m_nGauge++;
	}
	else if (m_nGaugeDest < m_nGauge)
	{
		m_nGauge--;
	}


	if (m_nGauge > MAX_GAUGE)
	{
		m_nGauge = MAX_GAUGE;
	}
	else if (m_nGauge < 0)
	{
		m_nGauge = 0;
	}

	float fRatio = (float)m_nGauge / MAX_GAUGE;

	m_pGauge->SetRatio(fRatio);

	m_pGauge->Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CAdrenaline_Gauge::Draw()
{
	//描画処理
	m_pGauge->Draw();
}

void CAdrenaline_Gauge::AddGauge(int nAdd)
{
	m_nGaugeDest += nAdd;
}

float CAdrenaline_Gauge::GetRateCalculation()
{
	return (float)m_nGauge / MAX_GAUGE;
}

