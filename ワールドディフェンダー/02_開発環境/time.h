//=============================================================================
//
// time.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _TIME_H_			//���̃}�N����`������Ȃ�������
#define _TIME_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CStatGauge;
class C2DPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTime : public CObject
{
private:
	static const int INIT_TIME = 3600;
	static const D3DXVECTOR3 CRAYFISH_POS;
	static const D3DXVECTOR3 EARTH_POS;
	static const int CRAYFISH_ANGLE_CNT = 60;
	static const int CRAYFISH_ANGLE_NUM = 4;
	static const float CRAYFISH_ANGLE[CRAYFISH_ANGLE_NUM];
public:
	CTime();
	~CTime() override;

	HRESULT Init() override;
	HRESULT Init(int nTimeMax);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool CheckTime(int nTime = 0);

	int GetTime();

	void SetTimeUpdate(bool bUpdate) { m_bUpdate = bUpdate; }
private:
	C2DPolygon* m_pCrayfish;
	C2DPolygon* m_pEarth;
	C2DPolygon* m_pWarningBorder;
	CStatGauge* m_pTimeGauge;
	bool m_bUpdate;
	int m_nChangeAngleCnt;
	int m_nChangeAngleNum;
};

#endif