//=============================================================================
//
//　アドレナリンゲージ処理 [adrenaline_gauge.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _ADRENALINE_GAUGE_H_			//このマクロ定義がされなかったら
#define _ADRENALINE_GAUGE_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class C2D6Polygon;
class CStatGauge;

//*****************************************************************************
// クラス定義
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

	HRESULT Init() override;//初期化
	HRESULT Init(D3DXVECTOR3 Pos);//初期化(Posあり)
	void Uninit() override;//終了処理
	void Update() override;//更新処理
	void Draw() override;//描画処理

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
