//=============================================================================
//
//　瓦礫の処理 [ballast_acquired.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _BALLAST_ACQUIRED_H_			//このマクロ定義がされなかったら
#define _BALLAST_ACQUIRED_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "ballast.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBallast_Acquired : public CBallast
{
private:
	static const float GRAVITY;
	static const float MOVE_MAGNIFICATION;
	static const float MOVE_ATTENUATION;

public:
	CBallast_Acquired();
	~CBallast_Acquired() override;

	void Update() override;//更新処理
	
	void SetVec(D3DXVECTOR3 Vec);
	D3DXVECTOR3 GetVec() {return m_Vec; }

private:
	D3DXVECTOR3 m_Vec;
	bool m_bFallenGroundOnce;
};

#endif
