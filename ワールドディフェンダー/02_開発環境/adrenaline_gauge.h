//=============================================================================
//
//�@�A�h���i�����Q�[�W���� [adrenaline_gauge.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _ADRENALINE_GAUGE_H_			//���̃}�N����`������Ȃ�������
#define _ADRENALINE_GAUGE_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class C2D6Polygon;
class CStatGauge;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAdrenaline_Gauge : public CObject
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const float INIT_HEIGHT;
	static const float INIT_WIDTH;
	static const int MAX_GAUGE = 100;
	static const int AUTOMATIC_DECREASE = 1;
	static const int COUNTER_MAX = 100;
public:
	CAdrenaline_Gauge();
	~CAdrenaline_Gauge() override;

	HRESULT Init() override;//������
	HRESULT Init(D3DXVECTOR3 Pos);//������(Pos����)
	void Uninit() override;//�I������
	void Update() override;//�X�V����
	void Draw() override;//�`�揈��

	void AddGauge(int nAdd);

	float GetRateCalculation();

	int GetGaugeValue() { return m_nGauge; }

private:
	CStatGauge* m_pAdrenaline_Gauge;
	C2D6Polygon* m_pGauge;
	D3DXVECTOR3 m_Pos;
	int m_nGauge;
	int m_nGaugeDest;
	int m_nCounter;
};

#endif
