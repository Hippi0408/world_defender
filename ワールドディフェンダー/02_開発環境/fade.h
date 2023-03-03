//=============================================================================
//
// fade.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _FADE_H_			//このマクロ定義がされなかったら
#define _FADE_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class C2DPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade : public CObject
{
private:
	
public:
	CFade();
	~CFade() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void FadeOutIn();
	void FadeOut() { m_bFadeUse = true; }
	bool CurrentFadeState() { return m_bFadeEnd; }

private:
	C2DPolygon *m_pFade;
	D3DXCOLOR m_FadeColor;
	bool m_bFadeUse;
	bool m_bFadeEnd;
};

#endif