//=============================================================================
//
// adrenaline_item.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _ADRENALINE_ITEM_H_			//このマクロ定義がされなかったら
#define _ADRENALINE_ITEM_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "3dobject.h"
#include <list>

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAdrenalineItem : public C3DObject
{
private:
	static const float RADIUS;
public:
	CAdrenalineItem();
	~CAdrenalineItem() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	static HRESULT AllInit();
	static void AllUninit();
	static void AllUpdate();
	static void AllDraw();

	static void DropAdrenalineItem(D3DXVECTOR3 pos);

	static bool PlayerCollision(D3DXVECTOR3 pos);

	static D3DXVECTOR3 AllGetLightVec() { return m_LightVec; }
	static void AllSetLightVec(D3DXVECTOR3 LightVec) { m_LightVec = LightVec; }

private:
	//アドレナリンアイテムのリスト
	static std::list<CAdrenalineItem*> m_AdrenalineItemList;
	static int m_nModelNum;
	static D3DXVECTOR3 m_LightVec;
};

#endif