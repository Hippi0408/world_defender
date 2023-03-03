//=============================================================================
//
// statgauge.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "statgauge.h"
#include "gauge.h"
#include <assert.h>

//*****************************************************************************
// �R���X�g���N�^
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
// �f�X�g���N�^
//*****************************************************************************
CStatGauge::~CStatGauge()
{
}

//*****************************************************************************
// ������
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
// �I������
//*****************************************************************************
void CStatGauge::Uninit()
{
	if (m_pStatGauge != nullptr)
	{
		m_pStatGauge = nullptr;
	}
}

//*****************************************************************************
// �X�V����
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

	// �����ƃe�N�X�`���̃����N
	if (m_bTextureChange)
	{
		IncreaseAndDecreaseAndTexture();
	}

	m_pStatGauge->Update();

	m_TopPos = m_Pos;

	m_TopPos.y -= m_pStatGauge->GetHeightElasticity();

}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CStatGauge::Draw()
{
}

//*****************************************************************************
// ���C�t�o�[�̍쐬
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
// �_���[�W����
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
// �Q�[�W�̏�Ԏ擾
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
// �����ƃe�N�X�`���̃����N
//*****************************************************************************
void CStatGauge::IncreaseAndDecreaseAndTexture()
{
	//�Q�[�W�̍ő卂���ƌ��݂̍���
	float fHeight, fHeightMax;

	//���݂̍���
	fHeight = (float)m_nValue;

	//�ő卂��
	fHeightMax = (float)m_nValueMax;


	//�����v�Z�p
	float fRatio;

	//�v�Z
	fRatio = fHeight / fHeightMax;

	//�����ۑ�
	m_fRatio = fRatio;

	//���݂�UVMin
	D3DXVECTOR2 UVMin = m_pStatGauge->GetUVSizeMin();

	//�ő�l�̕ύX
	UVMin.y = 1.0f - fRatio;

	//�ۑ�
	m_pStatGauge->SetUVSizeMin(UVMin);

}

//*****************************************************************************
// �_���[�W����
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

