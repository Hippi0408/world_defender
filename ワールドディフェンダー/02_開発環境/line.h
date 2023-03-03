//=============================================================================
//
// ライン処理 [line.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _LINE_H_			//このマクロ定義がされなかったら
#define _LINE_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLine : public CObject
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int LINE_BUFF_MAX = 16;
	// 頂点フォーマット
	static const DWORD FVF_VERTEX_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);		//座標、カラー
public:
	CLine();
	~CLine() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXCOLOR col);

	void SetMtxWorld(D3DXMATRIX MtxWorld) { m_MtxWorld = MtxWorld; }

private:
	D3DXMATRIX m_MtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_SizeMax;
	D3DXVECTOR3 m_SizeMin;
	D3DXVECTOR3 m_Pos;
	D3DXCOLOR m_Color;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLine;//頂点バッファへのポインタ
};

#endif
