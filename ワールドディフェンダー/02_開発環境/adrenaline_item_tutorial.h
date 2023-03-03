//=============================================================================
//
// adrenaline_item_tutorial.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ADRENALINE_ITEM_TUTORIAL_H_			//このマクロ定義がされなかったら
#define _ADRENALINE_ITEM_TUTORIAL_H_			//2重インクルード防止のマクロ定義

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
class CAdrenalineItemTutorial : public C3DObject
{
private:
	static const float RADIUS;
public:
	CAdrenalineItemTutorial();
	~CAdrenalineItemTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// プレイヤーとのの当たり判定処理
	bool PlayerCollision(D3DXVECTOR3 pos);
private:
};

#endif