//=============================================================================
//
// time.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "time.h"
#include "statgauge.h"
#include "texture.h"
#include "2dpolygon.h"

const D3DXVECTOR3 CTime::CRAYFISH_POS = D3DXVECTOR3(0.0f, -40.0f, 0.0f);
const D3DXVECTOR3 CTime::EARTH_POS = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
const float CTime::CRAYFISH_ANGLE[] = { D3DXToRadian(45),0.0f, D3DXToRadian(-45),0.0f };
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTime::CTime()
{
	m_pTimeGauge = nullptr;
	m_pCrayfish = nullptr;
	m_pEarth = nullptr;
	m_pWarningBorder = nullptr;
	m_bUpdate = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTime::~CTime()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CTime::Init()
{
	if (FAILED(CTime::Init(INIT_TIME)))
	{
		return -1;
	}

	m_nChangeAngleCnt = 0;
	m_nChangeAngleNum = 0;
	return S_OK;
}

//*****************************************************************************
// 初期化(タイム設定あり)
//*****************************************************************************
HRESULT CTime::Init(int nTimeMax)
{
	//タイムゲージ
	m_pTimeGauge = new CStatGauge;
	if (FAILED(m_pTimeGauge->Init()))
	{
		return -1;
	}

	CStatGauge::SStatGauge StatGauge;

	StatGauge.pos = D3DXVECTOR3(70.0f, 850.0f, 0.0f);
	StatGauge.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	StatGauge.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	StatGauge.fXSize = 60.0f;
	StatGauge.fYSize = 700.0f;
	StatGauge.nValueMax = nTimeMax;
	StatGauge.nRecoveryCntMaqx = 0;
	StatGauge.nRecovery = -1;

	m_pTimeGauge->CreateStatBar(StatGauge);
	m_pTimeGauge->SetTextureNum(CTexture::LoadTexture("data/TEXTURE/タイムゲージ.png"));
	m_pTimeGauge->SetTextureChange(true);


	//ザリガニ
	m_pCrayfish = new C2DPolygon;

	if (FAILED(m_pCrayfish->Init()))
	{
		return -1;
	}

	//情報の設定
	int nIndex = CTexture::LoadTexture("data/TEXTURE/ザリガニ.png");
	m_pCrayfish->SetTextIndex(nIndex);
	m_pCrayfish->SetPos(m_pTimeGauge->GetTopPos() + CRAYFISH_POS);
	m_pCrayfish->SetDiagonalLine(200.0f, 200.0f);
	m_pCrayfish->SetPolygon();

	//地球
	m_pEarth = new C2DPolygon;

	if (FAILED(m_pEarth->Init()))
	{
		return -1;
	}

	//情報の設定
	nIndex = CTexture::LoadTexture("data/TEXTURE/Earth.png");
	m_pEarth->SetTextIndex(nIndex);
	m_pEarth->SetPos(m_pTimeGauge->GetPos() + EARTH_POS);
	m_pEarth->SetDiagonalLine(100.0f, 100.0f);
	m_pEarth->SetPolygon();

	//警告フレーム
	m_pWarningBorder = new C2DPolygon;

	if (FAILED(m_pWarningBorder->Init()))
	{
		return -1;
	}

	//情報の設定
	nIndex = CTexture::LoadTexture("data/TEXTURE/警告フレーム.png");
	m_pWarningBorder->SetTextIndex(nIndex);
	m_pWarningBorder->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f));
	m_pWarningBorder->SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_pWarningBorder->SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.0f));
	m_pWarningBorder->SetPolygon();

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTime::Uninit()
{
	//タイムゲージ
	if (m_pTimeGauge != nullptr)
	{
		m_pTimeGauge->Uninit();
		delete m_pTimeGauge;
		m_pTimeGauge = nullptr;
	}

	//ザリガニ
	if (m_pCrayfish != nullptr)
	{
		m_pCrayfish->Uninit();
		delete m_pCrayfish;
		m_pCrayfish = nullptr;
	}

	//地球
	if (m_pEarth != nullptr)
	{
		m_pEarth->Uninit();
		delete m_pEarth;
		m_pEarth = nullptr;
	}
	
	//警告フレーム
	if (m_pWarningBorder != nullptr)
	{
		m_pWarningBorder->Uninit();
		delete m_pWarningBorder;
		m_pWarningBorder = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CTime::Update()
{
	//更新をしない
	if (!m_bUpdate)
	{
		return;
	}

	m_pTimeGauge->Update();

	D3DXVECTOR3 TopPos = m_pTimeGauge->GetTopPos();

	m_pCrayfish->SetPos(TopPos + CRAYFISH_POS);

	if (m_nChangeAngleCnt > CRAYFISH_ANGLE_CNT)
	{
		m_nChangeAngleCnt = 0;
		m_pCrayfish->SetRot(D3DXVECTOR3(0.0f,0.0f, CRAYFISH_ANGLE[m_nChangeAngleNum]));
		m_nChangeAngleNum = (m_nChangeAngleNum + 1) % CRAYFISH_ANGLE_NUM;
	}
	else
	{
		m_nChangeAngleCnt++;
	}
	m_pCrayfish->Update();

	m_pEarth->SetPos(m_pTimeGauge->GetPos() + EARTH_POS);
	m_pEarth->Update();

	D3DXCOLOR WarningColor = m_pWarningBorder->GetColor();

	WarningColor.a = 1.0f - m_pTimeGauge->GetRatio();

	m_pWarningBorder->SetColor(WarningColor);

	m_pWarningBorder->Update();
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTime::Draw()
{
	m_pWarningBorder->Draw();
	m_pEarth->Draw();
	m_pCrayfish->Draw();
	
}

bool CTime::CheckTime(int nTime)
{
	return m_pTimeGauge->ValueCheck(nTime);
}

int CTime::GetTime()
{
	return m_pTimeGauge->GetValue();
}
