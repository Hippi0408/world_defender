//=============================================================================
//
// gauge.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _GAUGE_H_			//このマクロ定義がされなかったら
#define _GAUGE_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "elasticitypolygon.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class C2DPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGauge : public CElasticityPolygon
{
private:
	static const int MAX_GAUGE = 16;
public:
	CGauge();
	~CGauge();

	void Update() override;
	void SetNumerics(int nInitialValue, int nMax);
	void AddNumerics(int nAddNumerics);
	void ChangeNumerics(int nNumerics);

	static CGauge *Create();
	static void AllUpdate();
	static void AllUninit();
	static void AllDraw();

private:
	int m_nNumerics;
	int m_nNumericsMax;
	float m_fSizeMax;

	static CGauge *m_pGauge[MAX_GAUGE];
};

#endif