//=============================================================================
//
// number.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _NUMBER_H_			//このマクロ定義がされなかったら
#define _NUMBER_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "2dpolygon.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
private:
	// ポリゴンの幅
	static const float SIZE_NUMBER;
	static const int NUMBER_MAX = 8;
public:

	CNumber();
	~CNumber();

	HRESULT Init();
	void Uninit();
	void Draw();

	void SetNumber(D3DXVECTOR3 pos, int nDigit, float fSize = SIZE_NUMBER,int nNumerical = 0);
	void SetNumberPos(D3DXVECTOR3 pos);
	void AddNumberPos(D3DXVECTOR3 add);
	void SetNumerical(int nNumerical);

	D3DXVECTOR3 GetPos() { return m_Pos; }

private:
	C2DPolygon *m_pNum[NUMBER_MAX];
	int m_nDigit;
	int m_nNumerical;
	int m_nTexIndex;
	float m_fSize;
	D3DXVECTOR3 m_Pos;
};

#endif