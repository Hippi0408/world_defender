//=============================================================================
//
// gauge.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "gauge.h"
#include "2dpolygon.h"

CGauge *CGauge::m_pGauge[CGauge::MAX_GAUGE] = {};
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGauge::CGauge()
{
	m_nNumerics = 0;
	m_nNumericsMax = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGauge::~CGauge()
{
}

//=============================================================================
// �X�V
//=============================================================================
void CGauge::Update()
{
	CElasticityPolygon::Update();
}

//=============================================================================
// ���l�ƃQ�[�W�̃T�C�Y�ݒ�
//=============================================================================
void CGauge::SetNumerics(int nInitialValue, int nMax)
{
	m_nNumerics = nInitialValue;
	m_nNumericsMax = nMax;
}

//=============================================================================
// ���l�ƃQ�[�W�̃T�C�Y�ύX
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
// �Q�[�W�̍쐬
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
// ���ׂĂ̍X�V
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
// ���ׂĂ̏I��
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
// ���ׂĂ̕`��
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