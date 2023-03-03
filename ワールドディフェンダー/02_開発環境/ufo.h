//=============================================================================
//
// ufo.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _UFO_H_			//このマクロ定義がされなかったら
#define _UFO_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "3dobject.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUfo : public C3DObject
{
private:
	static const int MOVE_COUNTER_MAX = 120;
	static const float POP_RANGE;
	static const float MOVING_DISTANCE;
	static const float POP_POS_Y;
	static const int POP_INTERVAL_MAX = 120;
public:
	CUfo();
	~CUfo() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void EnemyPop();

private:
	int m_nMoveCounter;
	int m_nPopInterval;
};

#endif