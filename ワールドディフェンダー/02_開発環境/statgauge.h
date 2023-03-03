//=============================================================================
//
// statgauge.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _STAT_GAUGE_H_			//このマクロ定義がされなかったら
#define _STAT_GAUGE_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CGauge;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStatGauge
{
private:

public:

	enum GaugeState
	{
		NormalGauge = 0,
		BreakTime,
		GaugeBreak,
		GaugeMax
	};


	struct SStatGauge
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR color;
		float fXSize;
		float fYSize;
		int nRecoveryCntMaqx;
		int nRecovery;
		int nValueMax;
	};




	CStatGauge();
	~CStatGauge();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	int GetValue() { return m_nValue; }
	void SetValue(int nStatGauge) { m_nValue = nStatGauge; }
	void CreateStatBar(SStatGauge SStatGauge);

	void SetStatGauge(int nStatGauge);

	bool GetGaugeState(GaugeState GaugeState);
	bool ValueCheck(int nCheck = 0);

	void SetTextureNum(int nTexture);

	void SetTextureChange(bool bTextureChange) { m_bTextureChange = bTextureChange; }

	void IncreaseAndDecreaseAndTexture();

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetTopPos() { return m_TopPos; }

	float GetRatio() { return m_fRatio; }
private:
	int m_nCntRecovery;
	int m_nCntRecoveryMax;
	int m_nRecovery;
	int m_nTexture;
	GaugeState m_GaugeState;

	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_TopPos;

	CGauge *m_pStatGauge;
	int m_nValue;
	int m_nValueMax;
	bool m_bTextureChange;
	float m_fRatio; //現在のゲージの割合
};

#endif