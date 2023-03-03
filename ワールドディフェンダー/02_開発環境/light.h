//=============================================================================
//
// ライト処理 [light.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LIGHT_H_			//このマクロ定義がされなかったら
#define _LIGHT_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLight : public CObject
{
private:
	static const int LIGHT_MAX = 4;

public:
	CLight();
	~CLight() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetLightVec();

private:
	D3DLIGHT9 m_alight[LIGHT_MAX];//ライトの情報

};

#endif

